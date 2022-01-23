#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

enum
{
    SIZE = 12,
    NORM_BYTE = (1 << 8) - 1,
    LEFTOVER = (1 << 4) - 1,
    BYTE_NUM = 4
};

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    int f = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (f == -1) {
        fprintf(stderr, "open error\n");
        return 1;
    }
    uint32_t x, write_char;
    while (scanf("%u", &x) >= 1) {
        unsigned char buf[BYTE_NUM];
        int i = 0;
        buf[i++] = (x >> (SIZE + CHAR_BIT)) & LEFTOVER;
        buf[i++] = (x >> SIZE) & NORM_BYTE;
        buf[i++] = (x >> CHAR_BIT) & LEFTOVER;
        buf[i] = x & NORM_BYTE;
        if ((write_char = write(f, buf, sizeof(buf))) != sizeof(buf)) {
            fprintf(stderr, "write error\n");
            return 1;
        }
    }
    close(f);
    return 0;
}


            

       

