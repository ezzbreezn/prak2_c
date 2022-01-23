#include <stdio.h>
#include <ctype.h>

enum
{
    XOR_MASK = 1 << 3,
    AND_MASK = (~(1 << 2)),
    DIG_NUM = '9' - '0' + 1,
    LETTERS_NUM = 'z' - 'a' + 1
};

int
main(void)
{
    int c;
    while ((c = getchar()) != EOF) {
        if (isdigit(c)) {
            c = c - '0' + 1;
        } else if (islower(c)) {
            c = c - 'a' + DIG_NUM + 1;
        } else if (isupper(c)) {
            c = c - 'A' + DIG_NUM + LETTERS_NUM + 1;
        } else {
            continue;
        }
        c ^= XOR_MASK;
        c &= AND_MASK;
        if (c == 0) {
            c = '@';
        } else if (c == DIG_NUM + 2 * LETTERS_NUM + 1) {
            c = '#';
        } else if (c >= 1 && c <= DIG_NUM) {
            c = c - 1 + '0';
        } else if (c >= DIG_NUM + 1 && c <= DIG_NUM + LETTERS_NUM) {
            c = c - 1 - DIG_NUM + 'a';
        } else if (c >= DIG_NUM + LETTERS_NUM + 1 && c <= DIG_NUM + 2 * LETTERS_NUM) {
            c = c - 1 - LETTERS_NUM - DIG_NUM + 'A';
        }
        putchar(c);
    }
    return 0;
}

