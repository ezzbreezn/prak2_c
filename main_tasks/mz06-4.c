#include <stdio.h>
#include <time.h>

enum
{
    START_YEAR = 1900,
    NEW_START = 1925,
    MONTH = 10,
    DAY = 7,
    SECONDS = 24 * 60 * 60,
    DAYS = 30,
    MONTHS = 12,
    ARG_NUM = 3
};

int
main(int argc, char *argv[])
{ 
    long long y, m, d;
    struct tm new_start = {};
    new_start.tm_year = NEW_START - START_YEAR;
    new_start.tm_mon = MONTH - 1;
    new_start.tm_mday = DAY;
    new_start.tm_isdst = 1;
    long long start;
    if ((start = mktime(&new_start)) == -1) {
        fprintf(stderr, "cannot represent as calendar time\n");
    }
    while (scanf("%lld%lld%lld", &y, &m, &d) == ARG_NUM) {
        struct tm cur_time = {};
        cur_time.tm_year = y - START_YEAR;
        cur_time.tm_mon = m - 1;
        cur_time.tm_mday = d;
        cur_time.tm_isdst = -1;
        long long cur;
        if ((cur = mktime(&cur_time)) == -1) {
            fprintf(stderr, "cannot represent as calendar time\n");
        }
        long long converted = cur - start;
        y = converted / (SECONDS * DAYS * MONTHS);
        m = (converted % (SECONDS * DAYS * MONTHS)) / (SECONDS * DAYS);
        d = ((converted % (SECONDS * DAYS * MONTHS)) % (SECONDS * DAYS)) / SECONDS;
        printf("%lld %lld %lld\n", y + 1, m + 1, d + 1);
    }
    return 0;
}
