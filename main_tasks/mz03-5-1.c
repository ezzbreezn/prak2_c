#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

enum
{
    BUF_SIZE = 4096,
    DIG_NUM = 10
};

int
main(int argc, char *argv[])
{
    char *file = NULL;
    size_t len = 0;
    ssize_t read_path;
    read_path = getline(&file, &len, stdin);
    if (read_path == -1) {
        fprintf(stderr, "read error\n");
        return 1;
    }
    int j = 0;
    while (file[j] != EOF && file[j] != '\0' && file[j] != '\r' && file[j] != '\n') {
        ++j;
    }
    file[j] = '\0';
    unsigned long long digits[DIG_NUM] = {};
    unsigned char buf[BUF_SIZE];

    FILE *f = fopen(file, "r");
    if (f != NULL) {
        size_t rc;
        while ((rc = fread(buf, sizeof(unsigned char), sizeof(buf), f)) > 0) {
            for (int i = 0; i < rc; ++i) {
                if (isdigit(buf[i])) {
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

