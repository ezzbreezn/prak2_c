#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

enum
{
    KIBIBYTE = 1023
};

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
    }
    unsigned long long ans = 0;
    for (int i = 1; i < argc; ++i) {
        struct stat buf = {};
        if (lstat(argv[i], &buf) != -1 && S_ISREG(buf.st_mode) && 
                buf.st_nlink == 1 && ((buf.st_size & KIBIBYTE) == 0)) {
            ans += buf.st_size;
        }
    }
    printf("%llu\n", ans);
    return 0;
}
