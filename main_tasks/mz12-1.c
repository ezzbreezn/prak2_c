#include <limits.h>

STYPE bit_reverse(STYPE value) {
    int size = sizeof(STYPE) * CHAR_BIT;
    UTYPE uvalue = value, ans = 0;
    for (int i = 0; i < size; ++i) {
        ans |= (uvalue & 1);
        if (i == size - 1) {
            break;
        }
        ans <<= 1;
        uvalue >>= 1;
    }
    return (STYPE) ans;
}

