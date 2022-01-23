#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

struct tm *
read_time(int *fd)
{
    time_t timeval;
    read(fd[0], &timeval, sizeof(timeval));
    close(fd[0]);
    close(fd[1]);
    return localtime(&timeval);
}

enum
{
    START_YEAR = 1900
};

int
main(int argc, char *argv[])
{
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "cant build pipe\n");
        exit(1);
    }
    if (fork()) {
        wait(NULL);
        struct tm *ans = read_time(fd);
        printf("Y:%04d\n", ans->tm_year + START_YEAR);
        exit(0);
    } else {
        if (fork()) {
            wait(NULL);
            struct tm *ans = read_time(fd);
            printf("M:%02d\n", ans->tm_mon + 1);
            exit(0);
        } else {
            if (fork()) {
                wait(NULL);
                struct tm *ans = read_time(fd);
                printf("D:%02d\n", ans->tm_mday);
                exit(0);
            } else {
                time_t timeval = time(NULL);
                write(fd[1], &timeval, sizeof(timeval));
                write(fd[1], &timeval, sizeof(timeval));
                write(fd[1], &timeval, sizeof(timeval));
                close(fd[0]);
                close(fd[1]);
                exit(0);
            }
        }
    }
}
