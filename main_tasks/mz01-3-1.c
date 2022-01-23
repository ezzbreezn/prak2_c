#include <stdio.h>

enum
{
    NMAX = 2000
};

void
eratosfen_seive(int *not_primes)
{
    not_primes[0] = 1;
    not_primes[1] = 1;
    for (int i = 2; i * i < NMAX; ++i) {
        if (!not_primes[i]) {
            for (int j = i * i; j < NMAX; j += i) {
                not_primes[j] = 1;
            }
        }
    }
}

int
main(int argc, char *argv[])
{
    int n;
    int not_primes[NMAX] = {};
    eratosfen_seive(not_primes);
    if (scanf("%d", &n) <= 0) {
        fprintf(stderr, "%s", "input failed\n"); 
        return 1;
    }
    if (n <= 1 || n >= NMAX || not_primes[n]) {
        fprintf(stderr, "%s", "wrong value\n");
        return 1;
    }    
    int reverse[NMAX];
    reverse[1] = 1;
    for (int i = 2; i < n; ++i) {
        reverse[i] = (n - (n / i) * reverse[n % i] % n) % n;
    } 
    for (int c = 0; c < n; ++c) {
        for (int a = 1; a < n; ++a) {
            printf("%d ", c * reverse[a] % n);
        }
        printf("\n");
    }
    return 0;
}
