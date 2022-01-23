#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

enum
{
    ARGNUM = 3
};

int
main(int argc, char *argv[])
{
    if (argc <= ARGNUM) {
        fprintf(stderr, "wrong input\n");
        exit(1);
    }
    int32_t count, modulo;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &modulo);
    int pid1, pid3;
    int fd24[2];
    pipe(fd24);
    pid1 = fork();
    if (!pid1) {
        close(fd24[0]);
        int fd12[2];
        pipe(fd12);
        int pid2 = fork();
        if (pid2) {
            close(fd24[1]);
            close(fd12[0]);
            FILE *fin = fdopen(fd12[1], "w");
            for (unsigned long long i = 1; i <= count; ++i) {
                unsigned long long temp = (i % modulo) * (i % modulo);
                temp %= modulo;
                fprintf(fin, "%llu ", temp);
                fflush(fin);
            }
            fclose(fin);
            close(fd12[1]); 
            exit(0);
        }
        close(fd12[1]);
        dup2(fd12[0], 0);
        close(fd12[0]);
        dup2(fd24[1], 1);
        close(fd24[1]);
        execlp(argv[ARGNUM], argv[ARGNUM], NULL);
        _exit(1);
    }
    close(fd24[1]);
    pid3 = fork();
    if (!pid3) {
        int pid4 = fork();
        if (!pid4) {
            unsigned long long x;
            FILE *fout = fdopen(fd24[0], "r");
            while (fscanf(fout, "%llu", &x) >= 1) {
                printf("%llu\n", x);
                fflush(stdout);
            }
            fclose(fout);
            close(fd24[0]); 
            exit(0);
        }
        wait(NULL);
        exit(0);
    }
    close(fd24[0]);
    wait(NULL);
    wait(NULL);
    printf("0\n");
    exit(0);
}
