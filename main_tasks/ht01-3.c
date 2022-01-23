#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>

int
main(int argc, char *argv[])
{
    if (argc <= 3) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    unsigned long long rows, cols;
    if (sscanf(argv[2], "%llu", &rows) < 1) {
        fprintf(stderr, "sscanf error\n");
        return 1;
    }
    if (sscanf(argv[3], "%llu", &cols) < 1) {
        fprintf(stderr, "sscanf error\n");
        return 1;
    }
    int f = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0660);
    if (f == -1) {
        fprintf(stderr, "cant open file\n");
        return 1;
    }
    struct stat info;
    if (stat(argv[1], &info) == -1) {
        fprintf(stderr, "cant read file info\n");
        exit(0);
    }
    if (!S_ISREG(info.st_mode)) {
        fprintf(stderr, "cant map\n");
        exit(1);
    }
    off_t size = (rows * cols) << 2;
    if (truncate(argv[1], size) == -1) {
        fprintf(stderr, "truncation failed\n");
        exit(1);
    }
    int32_t *file = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
    if (file == MAP_FAILED) {
        fprintf(stderr, "map failed\n");
        exit(1);
    }
    close(f);
    int32_t x = 1;
    unsigned long long r = rows, c = cols, i = 0, j = 0, k;
    int flag = 1;
    while (r > 0 && c > 0) {
        if (flag == 1) {
            for (k = 0; k < c; ++k) {
                *(file + i * (cols) + j + k) = x;
                ++x;
            }
            j = j + k - 1;
            ++i;
            --r;
            flag = 2;
        } else if (flag == 2) {
            for (k = 0; k < r; ++k) {
                *(file + (i + k) * (cols) + j) = x;
                ++x;
            }
            i = i + k - 1;
            --j;
            --c;
            flag = 3;
        } else if (flag == 3) {
            for (k = 0; k < c; ++k) {
                *(file + i * cols + j - k) = x;
                ++x;
            }
            j = j - k + 1;
            --i;
            --r;
            flag = 4;
        } else if (flag == 4) {
            for (k = 0; k < r; ++k) {
                *(file + (i - k) * cols + j) = x;
                ++x;
            }
            i = i - k + 1;
            ++j;
            --c;
            flag = 1;
        }
    }
    
    munmap(file, size);
    return 0;
}
