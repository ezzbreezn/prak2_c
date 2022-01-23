#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *
relativize_path(const char *path1, const char *path2)
{
    int len1 = strlen(path1);
    int len2 = strlen(path2);
    char *ans = realloc(NULL, sizeof(char) * 2 * (len1 + len2));
    if (ans == NULL) {
        fprintf(stderr, "realloc error\n");
        return NULL;
    }
    int len = 0;
    const char *p1 = path1, *p2 = path2, *comm_pref1 = path1, *comm_pref2 = path2;
    const char *end = path1 + (len1 - 1);
    while (end != path1 && *end != '/') {
        --end;
    }
    while (*p1 != '\0' && *p1 == *p2) {
        if (*p1 == '/') {
            comm_pref1 = p1;
            comm_pref2 = p2;
        }
        ++p2;
        if (p1 == end) {
            break;
        } else {
            ++p1;
        }
    }
    if (*p2 == '\0' && p1 == end) {
        ans[len++] = '.';
        ans[len] = '\0';
        return ans;
    } else if (*p2 == '\0' && *p1 == '/') {
        comm_pref1 = p1;
        comm_pref2 = p2;
    }
    if (end != path1) {
        --end;
    }
    if (comm_pref1 != path1) {
        --comm_pref1;
    }
    int backcd = 0;
    while (end != comm_pref1) {
        backcd = 1;
        if (*end == '/') {
            ans[len++] = '.';
            ans[len++] = '.';
            ans[len++] = '/';
        }
        --end;
    }
    if (comm_pref1 == path1 && backcd) {
        ans[len++] = '.';
        ans[len++] = '.';
        ans[len++] = '/';
    }
    if (*comm_pref2 != '\0') {
        ++comm_pref2;
    }
    while (*comm_pref2 != '\0') {
        ans[len++] = *comm_pref2;
        ++comm_pref2;
    }
    if (ans[len - 1] == '/') {
        ans[len - 1] = '\0';
    } else {
        ans[len] = '\0';
    }
    return ans;
}

int
main(int argc, char *argv[])
{
    char *ans = relativize_path(argv[1], argv[2]);
    printf("%s\n", ans);
    return 0;
}
