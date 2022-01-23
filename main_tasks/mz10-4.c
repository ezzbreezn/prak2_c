#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

//int
//run_and_check(char *cmd, int fd1, int fd2)
//{
//    int status, pid = fork();
//    if (pid == -1) {
//        return 1;
//    }
//    if (!pid) {
//        if (dup2(fd1, 0) == -1) {
//           exit(1);
//        }
//        if (dup2(fd2, 1) == -1) {
//            exit(1);
//        }
//        if (close(fd1) == -1) {
//            exit(1);
//        }
//        if (close(fd2) == -1) {
//            exit(1);
//        }
//        execlp(cmd, cmd, NULL);
//        _exit(1);
//    }
//    waitpid(pid, &status, 0);
//    if (WIFEXITED(status) && !WEXITSTATUS(status)) {
//        return 0;
//    } else {
//        return 1;
//    }
//}


void
run_and_check(char *cmd, int fd1, int fd2)
{
    if (fd1 != 0) {
        if (dup2(fd1, 0) == -1) {
            exit(1);
        }
        if (close(fd1) == -1) {
            exit(1);
        }
    }
    if (fd2 != 1) {
        if (dup2(fd2, 1) == -1) {
            exit(1);
        }
        if (close(fd2) == -1) {
            exit(1);
        }
    }
    execlp(cmd, cmd, NULL);
    _exit(1);
}

void
kill_procs(int *pids, int num)
{
    for (int i = 0; i < num - 1; ++i) {
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
    //int pid, status;
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
        //waitpid(pid, &status, 0);
        //if (!(WIFEXITED(status) && !WEXITSTATUS(status))) {
        //    exit(1);
        //} else {
        //    exit(0);
        //}
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
    for (int i = 1; i < argc; ++i) {
        if (i == 1) {
            pid = fork();
            if (pid == -1) {
                error_termination(pids, i);
            } else if (!pid) {
                close(pipes[0][0]) == -1;
                close(pipes[0][1]) == -1;
                close(pipes[1][0]) == -1;
                run_and_check(argv[i], 0, pipes[1][1]);
            }
            //waitpid(pid, &status, 0);
            //if (!(WIFEXITED(status) && !WEXITSTATUS(status))) {
            //    free(pids);
            //    exit(1);
            //}
            pids[i - 1] = pid;
        } else if (i == argc - 1) {
            pid = fork();
            if (pid == -1) {
                error_termination(pids, i);
            } else if (!pid) {
                close(pipes[!(i % 2)][1]) == -1;
                close(pipes[i % 2][0]) == -1;
                close(pipes[i % 2][1]) == -1;
                run_and_check(argv[i], pipes[!(i % 2)][0], 1);
            }
            if (close(pipes[!(i % 2)][0]) == -1) {
                error_termination(pids, i);
            }
            if (close(pipes[!(i % 2)][1]) == -1) {
                error_termination(pids, i);
            }
            //waitpid(pid, &status, 0);
            //if (!(WIFEXITED(status) && !WEXITSTATUS(status))) {
            //    killpids(pids, argc);
            //    free(pids);
            //    exit(1);
            //}
            pids[i - 1] = i;
        } else {
            pid = fork();
            if (pid == -1) {
                error_termination(pids, i);
            } else if (!pid) {
                close(pipes[!(i % 2)][1]) == -1;
                close(pipes[i % 2][0]) == -1;
                run_and_check(argv[i], pipes[!(i % 2)][0], pipes[i % 2][1]);
            }
            if (close(pipes[!(i % 2)][0]) == -1) {
                error_termination(pids, i);
            }
            if (close(pipes[!(i % 2)][1]) == -1) {
                error_termination(pids, i);
            }
            if (pipe(pipes[!(i % 2)]) == -1) {
                error_termination(pids, i);
            }
            //waitpid(pid, &status, 0);
            //if (!(WIFEXITED(status) && !WEXITSTATUS(status))) {
            //    killpids(pids, argc);
            //    free(pids);
            //    exit(1);
            //}
            pids[i - 1] = i;
        }
    }
    for (int i = 0; i < argc - 1; ++i) {
        wait(NULL);
    }
    if (pids != NULL) {
        free(pids);
    }
    exit(0);
}
