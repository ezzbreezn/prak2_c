#include <stdio.h>

int 
bin_pow_mod(int a, int p, int n)
{
    if (p == 0) {
        return 1;
    }
    else if (p % 2 != 0) {
        return bin_pow_mod(a, p - 1, n) * a % n;
    }
    else {
        return bin_pow_mod(a, p / 2, n) * bin_pow_mod(a, p / 2, n) % n;
    }
}

int
main(void)
{
    int n;
    scanf("%d", &n);
    for (int c = 0; c < n; ++c) {
        for (int a = 1; a < n; ++a) {
            int b = bin_pow_mod(a, n - 2, n) * c % n;
            printf("%d ", b);
        }
        printf("\n");
    }
    return 0;
}

