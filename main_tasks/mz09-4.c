#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
        exit(1);
    }
    char script[PATH_MAX], bash_str[PATH_MAX * 2];
    int randnum = open("/dev/random", O_RDONLY);
    unsigned long long num;
    if (read(randnum, &num, sizeof(num)) != sizeof(num)) {
        fprintf(stderr, "cant read number\n");
        exit(1);
    }
    close(randnum);
    char *tmp_dir = getenv("XDG_RUNTIME_DIR");
    if (tmp_dir == NULL) {
        tmp_dir = getenv("TMPDIR");
    }
    if (tmp_dir != NULL) {
        snprintf(script, PATH_MAX, "%s/%llu", tmp_dir, num);
    } else {
        snprintf(script, PATH_MAX, "/tmp/%llu", num);
    }
    int f = open(script, O_WRONLY | O_CREAT | O_TRUNC, 0755);
    if (f == -1) {
        fprintf(stderr, "cant open file\n");
        exit(1);
    }
    int old_stdout = dup(1);
    if (old_stdout == -1) {
        fprintf(stderr, "dup fail\n");
        exit(1);
    }
    if (dup2(f, 1) == -1) {
        fprintf(stderr, "dup2 fail\n");
        exit(1);
    }
    close(f);
    printf("#!/usr/bin/python3\nprint(%s", argv[1]);
    fflush(stdout);
    for (int i = 2; i < argc; ++i) {
        printf("*%s", argv[i]);
        fflush(stdout);
    }
    printf(")\n");
    fflush(stdout);
    if (dup2(old_stdout, 1) == -1) {
        fprintf(stderr, "dup2 fail\n");
        exit(1);
    }
    close(old_stdout); 
    snprintf(bash_str, PATH_MAX * 2 + 6, "%s; rm %s", script, script);
    execlp("/bin/sh", "sh", "-c", bash_str, NULL);
    _exit(1);
}
