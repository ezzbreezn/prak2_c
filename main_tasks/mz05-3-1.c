#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

enum
{
    MAX_N = 32,
    MAX_W = 999
};

int
main(int argc, char *argv[])
{
    uint32_t n, s;
    int w;
    if (scanf("%"SCNu32, &n) < 1) {
        fprintf(stderr, "read error\n");
    }
    if (n < 1 || n > MAX_N) {
        fprintf(stderr, "wrong input\n");
    }
    if (scanf("%"SCNu32, &s) < 1) {
        fprintf(stderr, "read error\n");
    }
    if (scanf("%d", &w) < 1) {
        fprintf(stderr, "read error\n");
    }
    if (w < 1 || w > MAX_W) {
        fprintf(stderr, "wrong input\n");
    }
    uint32_t max = (uint32_t) 1 << (n - 1);
    uint32_t x = 0;
    while (x < max) {
        printf("|%*"PRIo32"|%*"PRIu32"|%*"PRIu32"|\n", w, x, w, x, w, x);
        x += s;
    }
    uint32_t y = x - max;
    if (n < MAX_N) {
        max <<= 1;
        while (x < max) {
            printf("|%*"PRIo32"|%*"PRIu32"|%*"PRId32"|\n", w, x, w, x, w, -y);
            x += s;
            y += s;
        }
    } else {
        uint32_t xprev = x;
        while (x >= xprev) {
            printf("|%*"PRIo32"|%*"PRIu32"|%*"PRId32"|\n", w, x, w, x, w, -y);
            xprev = x;
            x += s;
            y += s;
        }
    }
    return 0;
}
