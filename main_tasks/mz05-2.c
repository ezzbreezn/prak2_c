#include <stdio.h>

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
    }
    unsigned int x;
    char template[] = "rwxrwxrwx";
    int len = sizeof(template) - 1;
    for (int i = 1; i < argc; ++i) {
        if (sscanf(argv[i], "%o", &x) < 1) {
            fprintf(stderr, "read error\n");
            return 1;
        }
        unsigned int tb = 1 << (len - 1);
        int j = 0;
        while (tb) {
            if (x & tb) {
                putchar(template[j]);
            } else {
                putchar('-');
            }
            tb >>= 1;
            ++j;
        }
        putchar('\n');
    }
    return 0;
}
