#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

enum
{
    BUF_SIZE = 16
};

struct Record
{
    char buf[BUF_SIZE];
    int32_t price;
};

int
main(int argc, char *argv[])
{
    int32_t sec_max = 0, max = 0;
    int flag = -1, rc = 0;
    for (int i = 1; i < argc; ++i) {
        int f = open(argv[i], O_RDONLY);
        if (f == -1) {
            fprintf(stderr, "read error\n");
        } else {
            struct Record temp = {};
            while ((rc = read(f, &temp, sizeof(temp))) == sizeof(temp)) {
                if (flag == -1) {
                    max = temp.price;
                    flag = 0;
                } else if (flag == 0 && temp.price != max) {
                    if (temp.price > max) {
                        sec_max = max;
                        max = temp.price;
                    } else {
                        sec_max = temp.price;
                    }
                    flag = 1;
                } else {
                    if (temp.price > max) {
                        sec_max = max;
                        max = temp.price;
                    } else if (temp.price != max && temp.price > sec_max) {
                        sec_max = temp.price;
                    }
                }
            }
            close(f);
        }
    }
    uint32_t ans = sec_max;
    if (flag == 1) {
        if (sec_max < 0) {
            printf("-");
            ans = -ans;
        }
        printf("%u.%02u\n", ans / 100, ans % 100);
    }
    return 0;
}


