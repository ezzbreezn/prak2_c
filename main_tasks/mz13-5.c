#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <fcntl.h>

void
map_file(char *name, int32_t **p, unsigned long long id, unsigned long long *init_sizes, unsigned long long *sizes)
{
    struct stat info;
    lstat(name, &info);
    p[id] = mmap(NULL, info.st_size * sizeof(*p[id]), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    init_sizes[id] = info.st_size * sizeof(*p[id]);
    int32_t x;
    FILE *f = fopen(name, "r");
    unsigned long long size = 0;
    while (fscanf(f, "%d", &x) >= 1) {
        p[id][size] = x;
        ++size;
    }
    fclose(f);
    sizes[id] = size;
}

void
seq_intersection(int32_t **p, unsigned long long *sizes, unsigned long long i1, unsigned long long i2)
{
    unsigned long long pos1 = 0, pos2 = 0, pos = 0;
    while (pos1 < sizes[i1] && pos2 < sizes[i2]) {
        if (p[i1][pos1] == p[i2][pos2]) {
            p[i1 / 2][pos] = p[i2][pos2];
            ++pos1;
            ++pos2;
            ++pos;
        } else if (p[i1][pos1] > p[i2][pos2]) {
            while (pos2 < sizes[i2] && p[i1][pos1] > p[i2][pos2]) {
                ++pos2;
            }
        } else {
            while (pos1 < sizes[i1] && p[i1][pos1] < p[i2][pos2]) {
                ++pos1;
            }
        }
    }
    sizes[i1 / 2] = pos;
}

void
seq_copy(int32_t **p, unsigned long long *sizes, unsigned long long n)
{
    for (int i = 0; i < sizes[n - 1]; ++i) {
        p[n / 2][i] = p[n - 1][i];
    }
    sizes[n / 2] = sizes[n - 1];
}


int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        exit(0);
    }
    int32_t **p = realloc(NULL, (argc - 1) * sizeof(*p));
    unsigned long long *init_sizes = realloc(NULL, (argc - 1) * sizeof(*init_sizes));
    unsigned long long *sizes;
    sizes = mmap(NULL, (argc - 1) * sizeof(*sizes), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (int i = 1; i < argc;  ++i) {
        map_file(argv[i], p, i - 1, init_sizes, sizes);
    }
    int n = argc - 1;
    while (n > 1) {
        for (int i = 0; i < n - 1; i += 2) {
            if (!fork()) {
                seq_intersection(p, sizes, i, i + 1);
                exit(0);
            }
        } 
        for (int i = 0; i < n / 2; ++i) {
            wait(NULL);
        }
        if (n % 2 == 1) {
            seq_copy(p, sizes, n);
        }
        n = (n + 1) / 2;
    }
    for (int i = 0; i < sizes[0]; ++i) {
        if (i == sizes[0] - 1) {
            printf("%d\n", p[0][i]);
        } else {
            printf("%d ", p[0][i]);
        }
    }
    for (int i = 0; i < argc - 1; ++i) {
        munmap(p[i], init_sizes[i]);
    }
    munmap(sizes, (argc - 1) * sizeof(*sizes));
    free(p);
    free(init_sizes);
    exit(0);
}

