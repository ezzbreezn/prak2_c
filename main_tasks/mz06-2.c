#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>

enum
{
    PERM_CHECK = 1 << 6
};

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    unsigned long long num = 0;
    char suffix[] = ".exe";
    int sufflen = sizeof(suffix) - 1;;
    DIR *directory;
    struct dirent *dir;
    char path[PATH_MAX];
    char *slash = "/";
    if (!strcmp(argv[1], "/")) {
        slash = "";
    }
    if ((directory = opendir(argv[1])) == NULL) {
        fprintf(stderr, "cannot open the directory\n");
        printf("0\n");
        return 1;
    }
    while ((dir = readdir(directory)) != NULL) {
        if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) {
            continue;
        }
        if (snprintf(path, PATH_MAX, "%s%s%s", argv[1], slash, dir->d_name) < 0) {
            fprintf(stderr, "snprintf error\n");
            return 1;
        }
        struct stat info;
        if (stat(path, &info) == -1) {
            fprintf(stderr, "cannot read file info\n");
            continue;
        }
        if (S_ISREG(info.st_mode) && !access(path, X_OK)) {
            int len = strlen(path);
            if (len >= sufflen && !strcmp(suffix, path + (len - sufflen))) {
                ++num;
            }
        }
    }
    closedir(directory);
    printf("%llu\n", num);
    return 0;
}
