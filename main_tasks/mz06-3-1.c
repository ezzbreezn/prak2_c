#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
transform(char *path)
{
    char *p1 = path, *p2 = path, *p3 = path;
    int flag = 0, dots = 0, not_dots = 0;
    while (*p2 != '\0') {
        if (*p2 == '/') {
            if (!flag) {
                not_dots = 0;
                flag = 1;
                p3 = p2;
                *p1 = *p2;
                ++p2;
            } else {
                if (dots == 2 && !not_dots) {
                    if (p1 != path) {
                        --p1;
                        while (p1 != path && *p1 != '/') {
                            --p1;
                        }
                    }
                } else if (dots != 1 || not_dots) {
                    ++p1;
                    ++p3;
                    while (*p3 != '\0' && *p3 != '/') {
                        *p1 = *p3;
                        ++p1;
                        ++p3;
                    }
                }
                dots = 0;
                flag = 0;
            }
        } else if (*p2 == '.') {
            ++dots;
            ++p2;
        } else {
            not_dots = 1;
            ++p2;
        }
    }
    if (not_dots) {
        ++p1;
        ++p3;
        while (*p3 != '\0') {
            *p1 = *p3;
            ++p1;
            ++p3;
        }
    } else if (p1 == path) {
        *p1 = '/';
        ++p1;
    } else if (dots == 2) {
        --p1;
        while (p1 != path && *p1 != '/') {
            --p1;
        }
        if (p1 == path) {
            *p1 = '/';
            ++p1;
        }
    }
    *p1 = '\0';
}

char *
relativize_path(const char *path1, const char *path2)
{
    unsigned long long len1 = strlen(path1);
    unsigned long long len2 = strlen(path2);
    char *npath1 = realloc(NULL, sizeof(char) * 2 * len1);
    if (npath1 == NULL) {
        fprintf(stderr, "realloc error\n");
        return NULL;
    }
    char *npath2 = realloc(NULL, sizeof(char) * 2 * len2);
    if (npath2 == NULL) {
        fprintf(stderr, "realloc error\n");
        return NULL;
    }
    if (snprintf(npath1, len1 + 1, "%s\n", path1) < 0) {
        fprintf(stderr, "snprintf error\n");
        return NULL;
    }
    if (snprintf(npath2, len2 + 1, "%s\n", path2) < 0) {
        fprintf(stderr, "snprintf error\n");
        return NULL;
    }
    transform(npath1);
    transform(npath2);
    len1 = strlen(npath1);
    len2 = strlen(npath2);
    char *ans = realloc(NULL, sizeof(char) * 2 * (len1 + len2));
    if (ans == NULL) {
        fprintf(stderr, "realloc error\n");
        free(npath1);
        free(npath2);
        return NULL;
    }
    unsigned long long len = 0;
    char *p1 = npath1, *p2 = npath2, *comm_pref1 = npath1, *comm_pref2 = npath2;
    char *end = npath1 + (len1 - 1);
    while (end != npath1 && *end != '/') {
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
        free(npath1);
        free(npath2);
        return ans;
    } else if (*p2 == '\0' && *p1 == '/') {
        comm_pref1 = p1;
        comm_pref2 = p2;
    }
    if (end != npath1) {
        --end;
    }
    if (comm_pref1 != npath1) {
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
    if (comm_pref1 == npath1 && backcd) {
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
    free(npath1);
    free(npath2);
    return ans;
}

