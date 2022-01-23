#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

enum
{
    ARGNUM = 4,
    ERROR = 42
};

int
main(int argc, char *argv[])
{
    if (argc <= ARGNUM) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    int status;
    if (!fork()) {
        int f1 = open(argv[2], O_RDONLY);
        if (f1 == -1) {
            exit(ERROR);
        }
        int f2 = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660);
        if (f2 == -1) {
            exit(ERROR);
        }
        int f3 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if (f3 == -1) {
            exit(ERROR);
        }
        if (dup2(f1, 0) == -1) {
            exit(ERROR);
        }
        if (dup2(f2, 1) == -1) {
            exit(ERROR);
        }
        if (dup2(f3, 2) == -1) {
            exit(ERROR);
        }
        if (close(f1) == -1) {
            exit(ERROR);
        }
        if (close(f2) == -1) {
            exit(ERROR);
        }
        if (close(f3) == -1) {
            exit(ERROR);
        }
        execlp(argv[1], argv[1], NULL);
        exit(ERROR);
    }
    wait(&status);
    printf("%d\n", status);
    return 0;
}
