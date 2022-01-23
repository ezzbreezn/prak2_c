#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

volatile sig_atomic_t flag = 1;

void
handler(int s)
{
    flag = 0;
}

void
wait_signal(void)
{
    while (flag == 1) {
        pause();
    }
    flag = 1;
}

void
processing(int num, int *pipe1, unsigned long long maxnum)
{
    unsigned long long temp = 1;
    int pid;
    FILE *fin = fdopen(pipe1[0], "r");
    FILE *fout = fdopen(pipe1[1], "w");
    if (num == 1) {
        fscanf(fin, "%d", &pid);
        fprintf(fout, "%d ", getpid());
        fflush(fout);
        kill(pid, SIGUSR1);
        wait_signal();
        flag = 1;
    } else if (num == 2) {
        wait_signal();
        fscanf(fin, "%d", &pid);
        kill(pid, SIGUSR1);
        wait_signal();
        fscanf(fin, "%llu", &temp);
    }
    while (temp < maxnum) {
        printf("%d %llu\n", num, temp);
        fflush(stdout);
        ++temp;
        fprintf(fout, "%llu ", temp);
        fflush(fout);
        kill(pid, SIGUSR1);
        wait_signal();
        fscanf(fin, "%llu", &temp);
    }
    fprintf(fout, "%llu ", temp);
    fflush(stdout);
    kill(pid, SIGUSR1);
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
        exit(1);
    }
    unsigned long long maxnum;
    sscanf(argv[1], "%llu", &maxnum);
    int pipe1[2];
    pipe(pipe1);
    struct sigaction s = {};
    s.sa_handler = handler;
    s.sa_flags = SA_RESTART;
    sigemptyset(&s. sa_mask);
    sigaddset(&s.sa_mask, SIGUSR1);
    sigaction(SIGUSR1, &s, NULL);
    int pid1, pid2;
    pid1 = fork();
    if (!pid1) {
        processing(1, pipe1, maxnum);
    }
    pid2 = fork();
    if (!pid2) {
        processing(2, pipe1, maxnum);
    }
    FILE *t = fdopen(pipe1[1], "w");
    fprintf(t, "%d ", pid2);
    fflush(t);
    fclose(t);
    close(pipe1[0]);
    close(pipe1[1]);
    wait(NULL);
    wait(NULL);
    printf("Done\n");
    exit(0);
}
