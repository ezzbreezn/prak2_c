#include <stdio.h>
#include <time.h>
#include <errno.h>

enum
{
    START_YEAR = 1900,
    ARGNUM = 4,
    HALF = 6,
    ST = 6,
    SN = 0
};

int
main(int argc, char *argv[])
{
    time_t year, month, day;
    unsigned long long last;
    if (scanf("%ld-%ld-%ld %llu", &year, &month, &day, &last) != ARGNUM) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    last *= 2;
    struct tm start = {};
    start.tm_year = year - START_YEAR;
    start.tm_mon = month - 1;
    start.tm_mday = day;
    start.tm_isdst = -1;
    errno = 0;
    if (mktime(&start) == -1 && errno) {
        fprintf(stderr, "cant reprsent as calendar time\n");
        return 1;
    }
    start.tm_mon += HALF;
    errno = 0;
    if (mktime(&start) == -1 && errno) {
        fprintf(stderr, "cant reprsent as calendar time\n");
        return 1;
    }
    while (last != 0) {
        if (start.tm_wday > 0 && start.tm_wday < ST) {
            printf("%04d-%02d-%02d\n", start.tm_year + START_YEAR, start.tm_mon + 1, start.tm_mday);
        } else {
            struct tm temp = start;
            while (temp.tm_wday != 1) {
                ++temp.tm_mday;
                errno = 0;
                if (mktime(&temp) == -1 && errno) {
                    fprintf(stderr, "cant reprsent as calendar time\n");
                    return 1;
                }
            }
            printf("%04d-%02d-%02d\n", temp.tm_year + START_YEAR, temp.tm_mon + 1, temp.tm_mday);
        }
        start.tm_mon += HALF;
        errno = 0;
        if (mktime(&start) == -1 && errno) {
            fprintf(stderr, "cant reprsent as calendar time\n");
            return 1;
        }
        --last;
    }   
    return 0;
}

