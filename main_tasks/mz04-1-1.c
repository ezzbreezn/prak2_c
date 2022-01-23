#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

enum
{
    SIZE = 12,
    BYTE = 8,
    B = (1 << 12) - 1,
    ALL = (1 << 24) - 1,
    NORM_BYTE = (1 << 8) - 1,
    LEFTOVER = (1 << 4) - 1
};

int
main(int argc, char *argv[])
{
    uint32_t x, out;
    int f = open(argv[1], O_WRONLY | O_CREAT, 0600);
    while (scanf("%u", &x) >= 1) {
        out = 0;
        x &= ALL;
        unsigned char t = x >> (SIZE + BYTE);
        out |= t;
        t = (x >> SIZE) & NORM_BYTE;
        out |= t <<  BYTE ;
        t = (x >> BYTE) & LEFTOVER;
        out |= t << 2 * BYTE;
        t = x & NORM_BYTE;
        out |= t << 3 * BYTE;
        printf("%x\n", out);
        write(f, &out, sizeof(out));
    }
    close(f);
    return 0;
}
