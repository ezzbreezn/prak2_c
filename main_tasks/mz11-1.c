#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

enum
{
    SIGNUM = 5
};

void
handler(int s)
{
    static int count = 0;
    if (count == SIGNUM) {
        exit(0);
    }
    printf("%d\n", count);
    fflush(stdout);
    ++count;
}


int
main(int argc, char *argv[])
{
    struct sigaction s = {};
    s.sa_handler = handler;
    s.sa_flags = SA_RESTART;
    sigemptyset(&s.sa_mask);
    sigaddset(&s.sa_mask, SIGHUP);
    sigaction(SIGHUP, &s, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while(1) {
        pause();
    }
    exit(0);
}
