#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

enum
{
    DIG_NUM = 10,
    BUF_SIZE = 1024
};

int
main(int argc, char *argv[])
{
    char file[PATH_MAX + 1];
    unsigned long long digits[DIG_NUM] = {};
    if (fgets(file, sizeof(file), stdin) == NULL) {
        fprintf(stderr, "read error\n");
    }
    size_t len = strlen(file);
    if (len >= 2 && file[len - 1] == '\n' && file[len - 2] == '\r') {
        file[len - 2] = '\0';
    } else if (len >= 1 && file[len - 1] == '\n') {
        file[len - 1] = '\0';
    } else if (len >= 1 && file[len - 1] != '\0') {
        fprintf(stderr, "name too long\n");
    }
    FILE *f = fopen(file, "r");
    if (f != NULL) {
        unsigned char buf[BUF_SIZE];
        int rc;
        while ((rc = fread(buf, sizeof(buf[0]), sizeof(buf), f)) > 0) {
            for (int i = 0; i < rc; ++i) {
                if (isdigit((unsigned char) buf[i])) {
                    ++digits[buf[i] - '0'];
                }
            }
        }
        fclose(f);
    } else {
        fprintf(stderr, "read error\n");
    }
    for (int i = 0; i < DIG_NUM; ++i) {
        printf("%d %llu\n", i, digits[i]);
    }
    return 0;
}

