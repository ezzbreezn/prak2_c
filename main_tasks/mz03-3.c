#include <stdio.h>
#include <math.h>

enum
{
    MAX_RATE = 10000,
    MAX_CHANGE_RATE = 100,
    ROUND_MULT = 10000,
    FULL_PERCENTAGE = 100
};

int
main(int argc, char *argv[])
{    
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    double start_rate;
    if (sscanf(argv[1], "%lf", &start_rate) < 1) {
        fprintf(stderr, "read error\n");
        return 1;
    }
    start_rate = round(start_rate * ROUND_MULT) / ROUND_MULT;
    if (start_rate <= 0 || start_rate > MAX_RATE) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    double rate_change;
    for (int i = 2; i < argc; ++i) {
        if (sscanf(argv[i], "%lf", &rate_change) < 1) {
            fprintf(stderr,"read error\n");
            return 1;
        }
        rate_change = round(rate_change * ROUND_MULT) / ROUND_MULT;
        if (rate_change <= -MAX_CHANGE_RATE || rate_change >= MAX_CHANGE_RATE) {
            fprintf(stderr, "wrong input\n");
            return 1;
        }
        start_rate *= (FULL_PERCENTAGE + rate_change) / FULL_PERCENTAGE;
        start_rate = round(start_rate * ROUND_MULT) / ROUND_MULT;
    }
    printf("%.4lf\n", start_rate);
    return 0;
}
