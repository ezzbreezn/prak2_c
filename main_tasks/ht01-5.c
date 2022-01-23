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
    D_NAME_SIZE = 256
};

void
strings_sort(char **strings, unsigned long long size)
{
    unsigned long long i, j, flag;
    for (j = 1; j < size; ++j) {
        flag = 0;
        for (i = 0; i < size - j; ++i) {
            if (strcasecmp(strings[i], strings[i + 1]) > 0) {
                char *temp = strings[i];
                strings[i] = strings[i + 1];
                strings[i + 1] = temp;
                flag = 1;
            }
        }
        if (!flag) {
            break;
        }
    } 
}
    

void
print(char *dirname, char *root, char *name)
{
    char path[PATH_MAX];
    DIR *directory;
    const char *slash = "/";
    if (!strcmp(dirname, "/")) {
        slash = "";
    }
    if ((directory = opendir(dirname)) == NULL) {
        fprintf(stderr, "opendir error\n");
        return;
    } else {
        if (strcmp(dirname, root)) {
            printf("cd %s\n", name);
        }
        struct dirent *content;
        unsigned long long size = MAX_SIZE, act_size = 0;
        char **names = realloc(NULL, sizeof(char *) * size);
        if (names == NULL) {
            fprintf(stderr, "realloc error\n");
            return;
        }
        char **fnames = realloc(NULL, sizeof(char *) * size);
        if (fnames == NULL) {
            for (unsigned long long i = 0; i < size; ++i) {
                free(names[i]);
            }
            free(names);
            fprintf(stderr, "realloc error\n");
            return;
        }
        for (unsigned long long j = 0; j < size; ++j) {
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
        while ((content = readdir(directory)) != NULL) {
            if (strcmp(content->d_name, ".") && strcmp(content->d_name, "..")) {
                if (snprintf(path, PATH_MAX, "%s%s%s", dirname, slash, content->d_name) < 1) {
                    fprintf(stderr, "snprintf error\n");
                    continue;
                }
                struct stat info;
                if (lstat(path, &info) == -1) {
                    fprintf(stderr, "lstat error\n");
                    continue;
                }
                if (S_ISDIR(info.st_mode)) {
                    if (act_size + 1 >= size) {
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
                        for (unsigned long long i = size / 2; i < size; ++i) {
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
        strings_sort(fnames, act_size);
        strings_sort(names, act_size);
        for (unsigned long long i = 0; i < act_size; ++i) {
            print(fnames[i], root, names[i]);
        }
        closedir(directory);
        for (unsigned long long i = 0; i < size; ++i) {
            free(names[i]);
            free(fnames[i]);
        }
        free(names);
        free(fnames);
        if (strcmp(dirname, root)) {
            printf("cd ..\n");
        }
    }
}


int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    print(argv[1], argv[1], NULL);
    return 0;
}
