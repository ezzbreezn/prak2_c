#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    int f = open(argv[1], O_RDONLY);
    if (f == -1) {
        fprintf(stderr, "unable to read the file\n");
        return 0;
    }
    uint16_t x, min, flag = 0;
    unsigned char buf[2];
    while (read(f, buf, sizeof(buf)) == sizeof(buf)) {
        x = buf[0];
        x <<= CHAR_BIT;
        x |= buf[1];
        if ((x & 1) == 0) {
            if (!flag) {
                flag = 1;
                min = x;
            } else if (x < min) {
                min = x;
            }
        }
    }
    close(f);
    if (flag) {
        printf("%hu\n", min);
    }
    return 0;
}
