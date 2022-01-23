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
    MAX_SIZE = 100,
    D_NAME_SIZE = 256
};

struct Fnode
{
    char *name;
    char *fname;
    struct Fnode *next;
};

void
strings_sort(struct Fnode *strings)
{
    struct Fnode *p1 = strings;
    if (p1 == NULL) {
        return;
    }
    while (p1 != NULL) {
        struct Fnode *p2 = p1->next;
        while (p2 != NULL) {
            if (strcasecmp(p1->name, p2->name) > 0) {
                char *str = p1->name;
                p1->name = p2->name;
                p2->name = str;
                str = p1->fname;
                p1->fname = p2->fname;
                p2->fname = str;
            }
            p2 = p2->next;
        }
        p1 = p1->next;
    }
}

    

void
print(char *dirname,  char *name)
{   
    DIR *directory;
    const char *slash = "/";
    if (!strcmp(dirname, "/")) {
        slash = "";
    }
    directory = opendir(dirname);
    if (directory == NULL) {
        return;
    } else {
        if (name != NULL) {
            printf("cd %s\n", name);
        }
        struct Fnode *strings = NULL, *cur_string = NULL;
        struct dirent *content;
        while ((content = readdir(directory)) != NULL) {
            if (strcmp(content->d_name, ".") && strcmp(content->d_name, "..")) {
                char path[PATH_MAX];
                if (snprintf(path, PATH_MAX, "%s%s%s", dirname, slash, content->d_name) < 1 ||
                        snprintf(path, PATH_MAX, "%s%s%s", dirname, slash, content->d_name) >= PATH_MAX) {
                    continue;
                }
                struct stat info;
                if (lstat(path, &info) < 0) {
                    continue;
                }
                if (S_ISDIR(info.st_mode)) {
                    struct Fnode *temp = realloc(NULL, sizeof(struct Fnode));
                    if (temp == NULL) {
                        fprintf(stderr, "realloc error\n");
                        return;
                    }
                    temp->next = NULL;
                    temp->fname = realloc(NULL, sizeof(char) * PATH_MAX);
                    if (temp->fname == NULL) {
                        fprintf(stderr, "realloc error\n");
                        free(temp);
                        return;
                    }
                    temp->name = realloc(NULL, sizeof(char) * D_NAME_SIZE);
                    if (temp->name == NULL) {
                        fprintf(stderr, "realloc error\n");
                        free(temp->fname);
                        free(temp);
                        return;
                    }
                    if (snprintf(temp->name, D_NAME_SIZE, "%s", content->d_name) < 1 || 
                            snprintf(temp->name, D_NAME_SIZE, "%s", content->d_name) >= PATH_MAX) {
                        continue;
                        free(temp->name);
                        free(temp->fname);
                        free(temp);
                    }
                    if (snprintf(temp->fname, PATH_MAX, "%s", path) < 1 ||
                            snprintf(temp->fname, PATH_MAX, "%s", path) >= PATH_MAX) {
                        continue;
                        free(temp->name);
                        free(temp->fname);
                        free(temp);
                    }
                    if (strings == NULL) {
                        strings = temp;
                        cur_string = temp;
                    } else {
                        cur_string->next = temp;
                        cur_string = cur_string->next;
                    }
                }
            }
        }
        closedir(directory);
        strings_sort(strings);
        cur_string = strings;
        while (cur_string != NULL) {
            print(cur_string->fname, cur_string->name);
            free(cur_string->fname);
            free(cur_string->name);
            cur_string = cur_string->next;
            free(strings);
            strings = cur_string;
        }
        if (name != NULL) {
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
    print(argv[1],  NULL);
    return 0;
}
