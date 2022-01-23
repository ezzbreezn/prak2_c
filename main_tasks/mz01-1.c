#include <stdio.h>

int
main(void)
{
    double x, y;
    int sc = scanf("%lf%lf", &x, &y);
    if (sc == 2) {
        printf("%d\n", y <= 5 && y >= 2 && x >= 1 && x <= 7 && y <= -x + 10); 
    }
    return 0;
}
