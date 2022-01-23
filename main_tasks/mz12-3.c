#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

enum
{
    SIGUSR1_CHANGE = 5,
    SIGUSR2_CHANGE = 4
};

volatile int counter = 0;

void
handler(int s)
{
    if (s == SIGUSR1) {
        counter += SIGUSR1_CHANGE;
        printf("%d %d\n", SIGUSR1, counter);
        fflush(stdout);
        if (counter < 0) {
            exit(0);
        }
    } else if (s == SIGUSR2) {
        counter -= SIGUSR2_CHANGE;
        printf("%d %d\n", SIGUSR2, counter);
        fflush(stdout);
        if (counter < 0) {
            exit(0);
        }
    }
}

int
main(int argc, char *argv[])
{
    struct sigaction s = {};
    sigemptyset(&s.sa_mask);
    sigaddset(&s.sa_mask, SIGUSR1);
    sigaddset(&s.sa_mask, SIGUSR2);
    s.sa_flags = SA_RESTART;
    s.sa_handler = handler;
    sigaction(SIGUSR1, &s, NULL);
    sigaction(SIGUSR2, &s, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        pause();
    }
    exit(0);
}
