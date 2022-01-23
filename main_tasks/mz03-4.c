#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>


enum
{
    BUF_SIZE = 16,
    DECIMAL = 10
};

int
main(int argc, char *argv[])
{
    unsigned char buf[BUF_SIZE];
    uint64_t sum = 0;
    uint64_t temp = 0;
    int start = 0, sign = 1, sign_flag = 0, read_char;
    while ((read_char = read(0, &buf, sizeof(buf))) > 0) {
        for (int i = 0; i < read_char; ++i) {
            if (isdigit(buf[i])) {
                if (!start) {
                    start = 1;
                    temp = 0;
                }
                if (!sign_flag) {
                    sign = 1;
                }
                temp *= DECIMAL;
                temp += buf[i] - '0';
            } else if (buf[i] == '+') {
                sign = 1;
                sign_flag = 1;
            } else if (buf[i] == '-') {
                sign = -1;
                sign_flag = 1;
            } else {
                temp *= sign;
                sum += temp;
                start = 0;
                sign_flag = 0;
                temp = 0;
            }
        }
    }
    if (start) {
        temp *= sign;
        sum += temp;
    }
    printf("%lld\n", (int64_t) sum);
    return 0;
}
