#include <stdio.h>
#include <string.h>
#include <ctype.h>

int
main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        int len = strlen(argv[i]);
        int j = 0, k = len - 1, flag = 1;
        while (j < k) {
            if (argv[i][j] != argv[i][k] || !(argv[i][j] >= 'a' && argv[i][j] <= 'z')) {
                flag = 0;
                break;
            }
            ++j;
            --k;
        }
        if (!(argv[i][j] >= 'a' && argv[i][j] <= 'z') || !(argv[i][k] >= 'a' && argv[i][k] <= 'z')) {
            flag = 0;
        }
        if (flag) {
            printf("%s\n", argv[i]);
        }
    }
    return 0;
}
