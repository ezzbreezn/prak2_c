#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
        exit(1);
    }
    unsigned long long n, success = 0;
    sscanf(argv[1], "%llu", &n);
    int *procs = realloc(NULL, (argc - 2 ) * sizeof(int));
    if (procs == NULL) {
        fprintf(stderr, "realloc error\n");
        exit(1);
    }
    char exe[PATH_MAX];
    unsigned long long i;
    for (i = 0; i + 2 < argc && i < n; ++i) {
        FILE *f = fopen(argv[i + 2], "r");
        if (f == NULL) {
            fprintf(stderr, "cant open file\n");
            free(procs);
            exit(1);
        }
        fgets(exe, sizeof(exe), f);
        exe[strlen(exe) - 1] = '\0';
        fclose(f);
        int pid = fork();
        if (!pid) {
            execlp(exe, exe, NULL);
            _exit(1);
        }
        procs[i] = pid;
    }
    int status;
    for (unsigned long long j = 0; j < i; ++j) {
        waitpid(procs[j], &status, 0);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            ++success;
        }
    }
    free(procs);
    if (i + 2 < argc) {
        for (; i + 2 < argc; ++i) {
            FILE *f = fopen(argv[i + 2], "r");
            if (f == NULL) {
                fprintf(stderr, "cant open file\n");
                exit(1);
            }
            fgets(exe, sizeof(exe), f);
            exe[strlen(exe) - 1] = '\0';
            fclose(f);
            int pid = fork();
            if (!pid) {
                execlp(exe, exe, NULL);
                _exit(1);
            }
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                ++success;
            }
        }
    }
    printf("%llu\n", success);
    fflush(stdout);
    exit(0);
}
