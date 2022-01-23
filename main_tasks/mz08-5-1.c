#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    int x, status, pid, parent_pid = getpid();
    printf("%d\n", getpid());
    while (scanf("%d", &x) >= 1) {
        pid = fork();
        if (pid == -1) {
            printf("-1\n");
            _exit(1);
        } else if (pid) {
            wait(&status);
            if (getpid() != parent_pid && (!WIFEXITED(status) || WEXITSTATUS(status))) {
                _exit(1);
            }
            if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                printf("%d %d\n", x, getpid());
            }
            return 0;
        }


    }
    return 0;
}
