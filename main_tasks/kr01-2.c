#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

enum
{
    BASE = 29,
    TEN = 10
};

int
main(int argc, char *argv[])
{
    char fname[PATH_MAX + 1];
    while (fgets(fname, PATH_MAX + 1, stdin) != NULL) {
        int len = strlen(fname);
        fname[len - 1] = '\0'; 
        int f = open(fname, O_RDONLY);
        if (f == -1) {
            printf("a\n");
            continue;
        }
        int64_t pnum = 0, num = 0, start = 0, sign = 1, of = 0;
        int c;
        while (read(f, &c, sizeof(char)) == sizeof(char)) {
            if (isdigit((unsigned char) c)) {
                if (!start) {
                    start = 1;
                }
                pnum = num;
                num *= BASE;
                if (num < pnum) {
                    of = 1;
                }
                num += ((unsigned char) c - '0');
                if (num < pnum) {
                    of = 1;
                }
            } else if ((unsigned char) c >= 'a' && (unsigned char) c <= 's') {
                if (!start) {
                    start = 1;
                }
                pnum = num;
                num *= BASE;
                if (num < pnum) {
                    of = 1;
                }
                num += ((unsigned char) c - 'a' + TEN);
                if (num < pnum) {
                    of = 1;
                }
            } else if ((unsigned char) c == '-') {
                sign = -1;
            } else {
                if (start && !of && (sign == 1 || num * sign != num)) {
                    printf("%ld\n", num * sign);
                }
                pnum = 0;
                num = 0;
                start = 0;
                of = 0;
                sign = 1;
            }
        }
        if (start) {
            if (sign == 1 || num * sign != num) {
                printf ("%ld\n", num * sign);
            }
        }
        close(f);
    }


    return 0;
}

