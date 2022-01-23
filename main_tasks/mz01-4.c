#include <ctype.h>

int 
mystrspccmp(const char *str1, const char *str2)
{
    const char *p1 = str1, *p2 = str2;
    while (1) {
        while (isspace((unsigned char) *p1)) {
            ++p1;
        }
        while (isspace((unsigned char) *p2)) {
            ++p2;
        }
        if (*p1 != *p2) {
            return (unsigned char) *p1 - (unsigned char) *p2;
        } else if (*p1 == '\0') {
            return 0;
        }
        ++p1;
        ++p2;
    }
    return 0;
}

