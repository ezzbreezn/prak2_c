#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

enum
{
    MAX_SIZE = 10000,
    D_NAME_SIZE = 256, //from struct dirent
    DEEP = 4
};

int
comp(const void *p1, const void *p2)
{
    char *s1 = *(char **) p1;
    char *s2 = *(char **) p2;
    return strcasecmp(s1, s2);
}

void
print(char *dirname, char *prev, int depth, unsigned long long z)
{
    if (depth == 0) {
        return;
    }
    char path[PATH_MAX];
    DIR *directory;
    char *slash = "/";
    if (!strcmp(dirname, "/")) {
        slash = "";
    }
    if ((directory = opendir(dirname)) == NULL) {
        fprintf(stderr, "opendir error\n");
        return;
    } else {
        struct dirent *content;
        unsigned long long size = MAX_SIZE, act_size = 0;
        //char *names[MAX_SIZE], *fnames[MAX_SIZE];
        char **names = realloc(NULL, sizeof(char *) * size);
        if (names == NULL) {
            fprintf(stderr, "realloc error\n");
            return;
        }
        char **fnames = realloc(NULL, sizeof(char *) * size);
        if (fnames == NULL) {
            for (int i = 0; i < size; ++i) {
                free(names[i]);
            }
            free(names);
            fprintf(stderr, "realloc error\n");
            return;
        }
        for (int j = 0; j < size; ++j) {
            names[j] = realloc(NULL, sizeof(char) * D_NAME_SIZE);
            if (names[j] == NULL) {
                fprintf(stderr, "realloc error\n");
                return;
            }
            fnames[j] = realloc(NULL, sizeof(char) * PATH_MAX);
            if (fnames[j] == NULL) {
                fprintf(stderr, "realloc error\n");
                return;
            }
        }
        //int len = 0;
        while ((content = readdir(directory)) != NULL) {
            if (strcmp(content->d_name, ".") && strcmp(content->d_name, "..")) {
                if (snprintf(path, PATH_MAX, "%s%s%s", dirname, slash, content->d_name) < 1) {
                    fprintf(stderr, "snprintf error\n");
                    continue;
                }
                struct stat info;
                if (stat(path, &info) == -1) {
                    fprintf(stderr, "lstat error\n");
                    continue;
                }
                if (S_ISREG(info.st_mode) && info.st_size <= z) {
                    char ans[PATH_MAX];
                    if (!strcmp(prev, "")) {
                        if (snprintf(ans, PATH_MAX, "%s", content->d_name) < 1) {
                            fprintf(stderr, "snprintf error\n");
                            continue;
                        }
                    } else {
                        if (snprintf(ans, PATH_MAX, "%s%s%s", prev, slash, content->d_name) < 1) {
                            fprintf(stderr, "snprintf error\n");
                            continue;
                        }
                    }
                    printf("%s\n", ans);
                } else if (S_ISDIR(info.st_mode)) {
                    if (act_size + 1 > size) {
                        size *= 2;
                        names = realloc(names, sizeof(char *) * size);
                        if (names == NULL) {
                            fprintf(stderr, "realloc error\n");
                            return;
                        }
                        fnames = realloc(fnames, sizeof(char *) * size);
                        if (fnames == NULL) {
                            fprintf(stderr, "realloc error\n");
                            return;
                        }
                        for (int i = size / 2; i < size; ++i) {
                            names[i] = realloc(NULL, sizeof(char) * D_NAME_SIZE);
                            if (names[i] == NULL) {
                                fprintf(stderr, "realloc error\n");
                                return;
                            }
                            fnames[i] = realloc(NULL, sizeof(char) * PATH_MAX);
                            if (fnames[i] == NULL) {
                                fprintf(stderr, "realloc error\n");
                                return;
                            }
                        }
                    }
                    if (snprintf(names[act_size], D_NAME_SIZE, "%s", content->d_name) < 1) {
                        fprintf(stderr, "snprintf error\n");
                    }
                    if (snprintf(fnames[act_size], PATH_MAX, "%s", path) < 1) {
                        fprintf(stderr, "snprintf error\n");
                    }
                    ++act_size; 
                }
            }
        }
        qsort(fnames, act_size, sizeof(fnames[0]), comp);
        qsort(names, act_size, sizeof(names[0]), comp);
        //printf("NAMES:\n");
        //for (int i = 0; i < len; ++i) {
        //    printf("%s\n", names[i]);
        //}
        //printf("FNAMES:\n");
        //for (int i = 0; i < len; ++i) {
        //    printf("%s\n", fnames[i]);
        //}
        char nprev[PATH_MAX];
        for (int i = 0; i < act_size; ++i) {
            //printf("cd %s\n", names[i]);
            if (!strcmp(prev, "")) {
                if (snprintf(nprev, PATH_MAX, "%s", names[i]) < 1) {
                    fprintf(stderr, "snprintf error\n");
                    continue;
                }
            } else { 
                if (snprintf(nprev, PATH_MAX, "%s%s%s", prev, slash, names[i]) < 1) {
                    fprintf(stderr, "snprintf error\n");
                    continue;
                }
            }
            print(fnames[i], nprev, depth - 1, z);
            //printf("cd ..\n");
        }
        closedir(directory);
        for (int i = 0; i < size; ++i) {
            free(names[i]);
            free(fnames[i]);
        }
        free(names);
        free(fnames);
    }
}


int
main(int argc, char *argv[])
{
    if (argc <= 2) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    unsigned long long msize;
    if (sscanf(argv[2], "%llu", &msize) < 1) {
        fprintf(stderr, "sscanf error\n");
        return 1;
    }
    print(argv[1], "", DEEP, msize);
    return 0;
}
