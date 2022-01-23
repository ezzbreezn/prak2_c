#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

void
run_and_check(char *cmd, int fd1, int fd2)
{
    if (!fork()) {
        if (fd1 != 0) {
            dup2(fd1, 0);
            close(fd1);
        }
        if (fd2 != 1) {
            dup2(fd2, 1);
            close(fd2);
        }
        execlp(cmd, cmd, NULL);
        _exit(1);
    }
}

int
main(int argc, char *argv[])
{
    if (argc <= 5) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "pipe fail\n");
        return 1;
    }
    if (!fork()) {
        close(fd[0]); 
        int f1 = open(argv[4], O_RDWR);
        if (f1 == -1) {
            fprintf(stderr, "cant open file\n");
            close(fd[1]);
            exit(1);
        }
        int status;
        run_and_check(argv[1], f1, fd[1]);
        close(f1);
        wait(&status);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            run_and_check(argv[2], 0, fd[1]);
        }
        close(fd[1]);
        wait(NULL);
        exit(0);
    }
    close(fd[1]);
    int f2 = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, 0660);
    if (f2 == -1) {
        fprintf(stderr, "cant open file\n");
        close(fd[0]);
        exit(1);
    }
    run_and_check(argv[3], fd[0], f2);
    close(fd[0]);
    close(f2);
    wait(NULL);
    wait(NULL);
    exit(0);
}
