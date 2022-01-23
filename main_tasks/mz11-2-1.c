#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>

enum
{
    SIGINT_ARRIVED = 1,
    SIGQUIT_ARRIVED = 0
};

volatile sig_atomic_t flag = 1;

void
handler(int s) {
    if (s == SIGINT) {
        flag = SIGINT_ARRIVED;
    } else if (s == SIGQUIT) {
        flag = SIGQUIT_ARRIVED;
    }
}

int
main(int argc, char *argv[])
{ 
    struct sigaction s = {};
    sigemptyset(&s.sa_mask);
    s.sa_handler = handler;
    s.sa_flags = SA_RESTART;
    sigaddset(&s.sa_mask, SIGINT);
    sigaddset(&s.sa_mask, SIGQUIT);
    sigaction(SIGINT, &s, NULL);
    sigaction(SIGQUIT, &s, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    uint32_t total = 0;
    int32_t temp = 0;
    while (scanf("%d", &temp) >= 1) {
        if (flag == SIGINT_ARRIVED) {
            total += temp;
            printf("%d\n", total);
            fflush(stdout);
        } else if (flag == SIGQUIT_ARRIVED) {
            total *= temp;
            printf("%d\n", total);
            fflush(stdout);
        }
    }
    return 0;
}
