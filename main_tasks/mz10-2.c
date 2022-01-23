#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>

int
main(int argc, char *argv[])
{
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "cant build pipe\n");
        exit(1);
    }
    if (fork()) {
        int32_t temp;
        while (scanf("%d", &temp) >= 1) {
            write(fd[1], &temp, sizeof(temp));
        }
        close(fd[0]);
        close(fd[1]);
        wait(NULL);
        exit(0);
    } else {
        if (fork()) {
            close(fd[0]);
            close(fd[1]);
            wait(NULL);
            exit(0);
        } else {
            close(fd[1]);
            int32_t temp;
            long long sum = 0;
            while (read(fd[0], &temp, sizeof(temp)) == sizeof(temp)) {
                sum += temp;
            }
            close(fd[0]);
            printf("%lld\n", sum);
            exit(0);
        }
    }
}
