#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

enum
{
    OK = 127,
    BASH_FAIL = 127,
    SIG = 128
};

int
mysys(const char *str) {
    int status;
    int pid = fork();
    if (pid == -1) {
        return -1;
    } else if (!pid) {
        execlp("/bin/sh", "sh", "-c", str, NULL);
        _exit(BASH_FAIL);
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) >= 0 && WEXITSTATUS(status) <= OK) {
        return WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        return SIG + WTERMSIG(status);
    }
    return 0;
}

