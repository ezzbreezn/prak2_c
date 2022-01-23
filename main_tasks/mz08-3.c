#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdlib.h>

enum
{
    BUFSIZE = 8,
    PROC_NUM = 3
};

int
main(int argc, char *argv[])
{
    char buf[BUFSIZE];
    int32_t x;

    for (int i = 1; i <= PROC_NUM; ++i) {
        if (!fork()) {
            read(0, buf, sizeof(buf));
            sscanf(buf, "%d", &x);
            printf("%d %d\n", i, x * x);
            exit(0);
        }
    }
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}
