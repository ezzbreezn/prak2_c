#include <stdio.h>
#include <time.h>
#include <errno.h>

enum
{
    START_YEAR = 1900,
    START = 1910,
    END = 2037,
    DECEMBER = 12,
    THURSDAY = 4,
    SEC = 2,
    FRTH = 4,
    DIVIDER = 3,
    WEEK = 7
};

int
main(int argc, char *argv[])
{
    time_t year;
    if (scanf("%ld", &year) < 1) {
        fprintf(stderr, "read error\n");
        return 1;
    }
    if (year < START || year > END) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    struct tm schedule = {};
    schedule.tm_year = year - START_YEAR;
    int next_year = year - START_YEAR + 1;
    schedule.tm_isdst = -1;
    errno = 0;
    if (mktime(&schedule) == -1 && errno) {
        fprintf(stderr, "canot represent as calendar time\n");
        return 1;
    }
    int flag = 0;
    while (schedule.tm_year != next_year) {
        int next_mon = schedule.tm_mon + 1;
        if (next_mon == DECEMBER) {
            next_mon = 0;
        }
        int num = 0;
        while (schedule.tm_mon != next_mon) {
            if (schedule.tm_wday == THURSDAY) {
                ++num;
                flag = 1;
                if ((num == SEC || num == FRTH) && schedule.tm_mday % DIVIDER != 0) {
                    printf("%d %d\n", schedule.tm_mon + 1, schedule.tm_mday);
                }
            }
            if (!flag) {
                ++schedule.tm_mday;
            } else {
                schedule.tm_mday += WEEK;
            }
            errno = 0;
            if (mktime(&schedule) == -1 && errno) {
                fprintf(stderr, "cannot represesnt as calendar time\n");
                return 1;
            }
        }
    }
    return 0;
}
