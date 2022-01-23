#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    int x, status, pid;
    pid = fork();
    if (pid == -1) {
        printf("-1\n");
        return 0;
    } else if (pid) { 
        wait(NULL);
        return 0;
    }
    while (scanf("%d", &x) >= 1) {
        pid = fork();
        if (pid == -1) {
            printf("-1\n");
            exit(1);
        } else if (pid) {
            wait(&status);
            if (!WIFEXITED(status) || WEXITSTATUS(status)) {
                exit(1);
            } else {
                printf("%d\n", x);
            }
            return 0;
        }
    }
    return 0;
}
            
