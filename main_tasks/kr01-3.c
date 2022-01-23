#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>

enum
{
    MASK = 511,
    SIZE = 9
};

int
main(int argc, char *argv[])
{
    uint16_t *addr, temp;
    sscanf(argv[2], "%p", &addr);  
    while (sscanf(argv[1], "%hx", &temp) == 1) {
        uint16_t num = temp >> SIZE;
        uint16_t val = *(addr + 2 * num);
        temp = val | (temp & MASK);
        uint16_t *ans = (uint16_t *) temp;
        printf("%hu\n", *ans);
    }
    return 0;
}
