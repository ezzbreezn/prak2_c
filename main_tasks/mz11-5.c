#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

volatile sig_atomic_t output = 0;

enum
{
    MAX_COUNT = 4
};

int
isprime(long long x)
{
    if (x < 2) {
        return 0;
    }
    for (long long i = 2; i * i <= x; ++i) {
        if (x % i == 0) {
            return 0;
        }
    }
    return 1;
} 

void
handler(int s)
{
    if (s == SIGINT) {
        static int count = 0;
        ++count;
        if (count == MAX_COUNT) {
            exit(0);
        }
        output = 1;
    } else if (s == SIGTERM) {
        exit(0);
    }
}

int
main(int argc, char *argv[])
{ 
    struct sigaction s = {};
    sigemptyset(&s.sa_mask);
    sigaddset(&s.sa_mask, SIGINT);
    sigaddset(&s.sa_mask, SIGTERM);
    s.sa_flags = SA_RESTART;
    s.sa_handler = handler;
    sigaction(SIGINT, &s, NULL);
    sigaction(SIGTERM, &s, NULL);
    long long low, high, temp = 0;
    scanf("%lld%lld", &low, &high);
    printf("%d\n", getpid());
    fflush(stdout);
    for (long long i = low; i < high; ++i) {
        if (isprime(i)) {
            temp = i;
        }
        if (output == 1) {
            printf("%lld\n", temp);
            fflush(stdout);
            output = 0;
        }
    }
    printf("-1\n");
    fflush(stdout);
    exit(0);
}
