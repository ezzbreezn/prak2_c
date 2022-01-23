#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    unsigned long long n;
    if (scanf("%llu", &n) < 1) {
        fprintf(stderr, "read error\n");
        return 1;
    }
    if (n <= 0) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    if (n == 1) {
        printf("1\n");
    } else {
        printf("1 ");
    }
    int i = 2;
    while (i <= n) {
        if (!fork()) {
            if (i < n) {
                printf("%d ", i);
            } else {
                printf("%d\n", i);
            }
        } else {
            wait(NULL);
            _exit(0);
        }
        ++i;
    }
    return 0;
}
