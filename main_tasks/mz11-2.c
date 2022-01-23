#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t total = 0;
int32_t temp = 0;

void
f1(void)
{
    total += temp;
    printf("%d\n", total);
    fflush(stdout);
}

void
f2(void)
{
    total *= temp;
    printf("%d\n", total);
    fflush(stdout);
}

void (*p)(void) = f1;

void
handler(int s) {
    if (s == SIGINT) {
        p = f1;
    } else if (s == SIGQUIT) {
        p = f2;
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
    while (scanf("%d", &temp) >= 1) {
        (*p)();
    }
    return 0;
}
