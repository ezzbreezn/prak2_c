#include <ctype.h>
#include <stddef.h>

int
parse_rwx_permissions(const char *str)
{
    if (str == NULL) {
        return -1;
    }
    int ans = 0;
    char template[] = "rwxrwxrwx";
    unsigned int mask = 1 << (sizeof(template) - 2);
    int i;
    for (i = 0; template[i] != '\0'; ++i) {
        if (str[i] != 'r' && str[i] != 'w' && str[i] != 'x' && str[i] != '-') {
            return -1;
        } else if (str[i] == template[i]) {
            ans |= mask;
        } else if (str[i] != '-') {
            return -1;
        }
        mask >>= 1;
    }
    if (str[i] != '\0') {
        return -1;
    } else {
        return ans;
    }
}

