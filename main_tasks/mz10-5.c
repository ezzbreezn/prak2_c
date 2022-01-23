#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

void
processing(int num, int *pipe1, int *pipe2, unsigned long long maxnum) 
{
    unsigned long long temp;
    FILE *fin = fdopen(pipe1[0], "r");
    FILE *fout = fdopen(pipe2[1], "w");
    while (fscanf(fin, "%llu", &temp) != -1 && temp < maxnum) {   
        printf("%d %llu\n", num, temp);
        fflush(stdout);
        ++temp;
        fprintf(fout, "%llu ", temp);
        fflush(fout);
    }
    exit(0);
}

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    unsigned long long maxnum;
    sscanf(argv[1], "%llu", &maxnum);
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1) {
        fprintf(stderr, "cant build pipe\n");
        exit(1);
    }
    if (pipe(pipe2) == -1) {
        fprintf(stderr, "cant build pipe\n");
        exit(1);
    }
    unsigned long long num = 1;
    int pid1, pid2;
    pid1 = fork();
    if (!pid1) {
        close(pipe1[1]);
        close(pipe2[0]);
        processing(1, pipe1, pipe2, maxnum);
    }
    pid2 = fork();
    if (!pid2) {
        close(pipe1[0]);
        close(pipe2[1]);
        processing(2, pipe2, pipe1, maxnum);
    }
    FILE *t = fdopen(pipe1[1], "w");
    fprintf(t, "%llu ", num);
    fflush(t);
    fclose(t);
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    wait(NULL);
    wait(NULL);
    printf("Done\n");
    exit(0);
}
