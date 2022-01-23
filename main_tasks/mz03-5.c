#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    FILENAME_SIZE = 100,
    DIG_NUM = 10,
    BUF_SIZE = 1024
};

int
main(int argc, char *argv[])
{
    unsigned long long size = FILENAME_SIZE;
    char *file = realloc(NULL, size * sizeof(char));
    if (file == NULL) {
        fprintf(stderr, "allocation error\n");
        return 1;
    } 
    unsigned long long digits[DIG_NUM] = {};
    int c;
    unsigned long long len = 0;
    while ((c = getchar()) != EOF && c != '\n' && c != '\r') {
        if (len + 2 > size) {
            size *= 2;
            file = realloc(file, size * sizeof(char));
            if (file == NULL) {
                fprintf(stderr, "allocation error\n");
                return 1;
            }
        }
        file[len++] = c;
    }
    if (len > 0) {
        if (len + 1 > size) {
            size *= 2;
            file = realloc(file, size * sizeof(char));
            if (file == NULL) {
                fprintf(stderr, "allocation error\n");
                return 1;
            }
        }
        file[len] = '\0';
        FILE *f = fopen(file, "r");
        if (f != NULL) {
            unsigned char buf[BUF_SIZE];
            int rc;
            while (!feof(f) && (rc = fread(buf, sizeof(buf[0]), sizeof(buf), f)) > 0) {
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
    }
    free(file);
    for (int i = 0; i < DIG_NUM; ++i) {
        printf("%d %llu\n", i, digits[i]);
    }
    return 0;
}
