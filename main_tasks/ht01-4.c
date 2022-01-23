#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>

enum
{
    ARGNUM = 3,
    MAX_ARG_SIZE = 64
};


int
main(int argc, char *argv[])
{
    if (argc <= ARGNUM) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    int signlen = strlen(argv[ARGNUM]);
    void *lib = dlopen(argv[1], RTLD_LAZY);
    if (lib == NULL) {
        fprintf(stderr, "cant open library\n");
        exit(0);
    }
    void *f = dlsym(lib, argv[2]);
    if (f == NULL) {
        fprintf(stderr, "cant find the function\n");
        exit(0);
    }
    unsigned char buf[MAX_ARG_SIZE] = {};
    int offset = 0;
    for (int i = 1; ARGNUM + i < argc && i < signlen; ++i) {
        if (argv[ARGNUM][i] == 'i') {
            sscanf(argv[ARGNUM + i], "%d", (int *)(buf + offset));
            offset += sizeof(int);
        } else if (argv[ARGNUM][i] == 'd') {
            sscanf(argv[ARGNUM + i], "%lf", (double *)(buf + offset));
            offset += sizeof(double);
        } else if (argv[ARGNUM][i] == 's') {
            *(char **)(buf + offset) = argv[ARGNUM + i];
            offset += sizeof(char *);
        }
    }
    unsigned int scd = 0;
    double fst = 0;
    __asm__(
            "finit\n"
            "pushl %%edi\n"
            "movl %%esp, %%edi\n"
            "pushl %%ebx\n"
            "andl $~15, %%esp\n"
            "movl %2, %%ebx\n"
            "addl $60, %%ebx\n"
            "movl $64, %%ecx\n"
".START:\n"
            "cmpl $0, %%ecx\n"
            "je .END\n"
            "pushl (%%ebx)\n"
            "subl $4, %%ebx\n"
            "subl $4, %%ecx\n"
            "jmp .START\n"
".END:\n"
            "call *%3\n"
            "fstpl %1\n"
            "movl %%eax, %0\n"
            "movl %%edi, %%esp\n"
            "subl $4, %%esp\n"
            "popl %%ebx\n"
            "popl %%edi\n"
            : "=r"(scd), "=m"(fst)
            : "r"(buf), "r"(f)
            : 
            );
    dlclose(lib);
    if (argv[ARGNUM][0] == 'i') {
        printf("%d\n", (int)scd);
    } else if (argv[ARGNUM][0] == 'd') {
        printf("%.10g\n", fst);
    } else if (argv[ARGNUM][0] == 's') {
        printf("%s\n", (char *)scd);
    }
    return 0;
}
