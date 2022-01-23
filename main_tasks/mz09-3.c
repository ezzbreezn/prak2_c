#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int
run_and_check(char *cmd)
{
    int pid = fork();
    if (pid == -1) {
        return 1;
    } else if (!pid) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    }
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && !WEXITSTATUS(status)) {
        return 0;
    } else {
        return 1;
    }
}

int
main(int argc, char *argv[])
{
    if (argc <= 3) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    return !((!run_and_check(argv[1]) || !run_and_check(argv[2])) && !run_and_check(argv[3]));
}
