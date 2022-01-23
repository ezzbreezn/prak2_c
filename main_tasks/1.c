#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int
main(int argc, char *argv[])
{
    off_t size = 0;
    struct stat buf;
    for (int i = 1; i < argc; ++i) {
        if (lstat(argv[i], &buf) == -1) {
            fprintf(stderr, "error\n");
        } else (S_ISREG(buf.st_mode) && !access(argv[i], W_OK)) {
            size += buf.st_size;
        }
    }
    printf("%ld\n", size);
    return 0;
} 
