#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>

void
intersection(FILE *in1, FILE *in2, FILE *out)
{
    int x1, x2;
    while (fscanf(in1, "%d", &x1) == 1) {
        printf("1 %d\n", x1);
        do {
            if (fscanf(in2, "%d", &x2) < 0) {
                return;
            }
            printf("2 %d\n", x2);
        } while (x2 < x1);
        
        if (x2 == x1) {
            printf("a\n");
            fprintf(out, "%d\n", x1);
        }
    }
}


void
processing(int32_t *f, int size) 
{
    for (int i = 0; i < size; ++i) {
        printf("%d\n", f[i]);
    }
}

int
main(int argc, char *argv[])
{
    //FILE *f1 = fopen(argv[1], "rw");
    int f1 = open(argv[1], O_RDWR);
    //FILE *f2 = fopen(argv[2], "r");
    //FILE *f3 = fopen(argv[3], "w");
    //intersection(f1, f2, f3);
    //fclose(f1);
    //fclose(f2);
    //fclose(f3);
    struct stat info = {};
    lstat(argv[1], &info);
    int32_t *ptr = mmap(NULL, info.st_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, f1, 0);
    processing(ptr, info.st_size);
    exit(0);
}

