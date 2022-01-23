#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

void
run_and_check(char *cmd, int fd1, int fd2)
{
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

void
kill_procs(int *pids, int num)
{
    for (int i = 0; i < num; ++i) {
        kill(pids[i], SIGKILL);
    }
}

void
error_termination(int *pids, int num)
{
    kill_procs(pids, num);
    free(pids);
    exit(1);
}

int
main(int argc, char *argv[])
{
    int pid;
    if (argc <= 1) {
        return 0;
    } else if (argc == 2) {
        pid = fork();
        if (pid == -1) {
            exit(1);
        } else if (!pid) {
            run_and_check(argv[1], 0, 1);
        }
        wait(NULL);
        exit(0);
    }
    int pipes[2][2];
    if (pipe(pipes[0]) == -1) {
        exit(1);
    }
    if (pipe(pipes[1]) == -1) {
        exit(1);
    }
    int *pids = realloc(NULL, (argc - 1) * sizeof(int));
    if (pids == NULL) {
        exit(1);
    }
    for (int i = 0; i < argc - 1; ++i) {
        if (i == 0) {
            pid = fork();
            if (pid == -1) {
                error_termination(pids, i);
            } else if (!pid) {
                close(pipes[0][0]);
                close(pipes[0][1]);
                close(pipes[1][0]);
                run_and_check(argv[i + 1], 0, pipes[1][1]);
            }
            pids[i] = pid;
        } else if (i == argc - 2) {
            pid = fork();
            if (pid == -1) {
                error_termination(pids, i);
            } else if (!pid) {
                close(pipes[i % 2][1]);
                close(pipes[!(i % 2)][0]);
                close(pipes[!(i % 2)][1]);
                run_and_check(argv[i + 1], pipes[i % 2][0], 1);
            }
            pids[i] = pid;
            if (close(pipes[i % 2][0]) == -1) {
                error_termination(pids, i + 1);
            }
            if (close(pipes[i % 2][1]) == -1) {
                error_termination(pids, i + 1);
            }
            if (close(pipes[!(i % 2)][0]) == -1) {
                error_termination(pids, i + 1);
            }
            if (close(pipes[!(i % 2)][1]) == -1) {
                error_termination(pids, i + 1);
            }            
        } else {
            pid = fork();
            if (pid == -1) {
                error_termination(pids, i);
            } else if (!pid) {
                close(pipes[i % 2][1]);
                close(pipes[!(i % 2)][0]);
                run_and_check(argv[i + 1], pipes[i % 2][0], pipes[!(i % 2)][1]);
            }
            pids[i] = pid;
            if (close(pipes[i % 2][0]) == -1) {
                error_termination(pids, i + 1);
            }
            if (close(pipes[i % 2][1]) == -1) {
                error_termination(pids, i + 1);
            }
            if (pipe(pipes[i % 2]) == -1) {
                error_termination(pids, i + 1);
            } 
        }
    }
    close(pipes[0][0]);
    close(pipes[0][1]);
    close(pipes[1][0]);
    close(pipes[1][1]);
    for (int i = 0; i < argc - 1; ++i) {
        wait(NULL);
    }
    if (pids != NULL) {
        free(pids);
    }
    exit(0);
}
