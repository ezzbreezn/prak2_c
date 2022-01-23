#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    char source[PATH_MAX];
    int32_t mod;
    sscanf(argv[2], "%d", &mod);
    int randgen = open("/dev/random", O_RDONLY);
    unsigned long long randnum;
    read(randgen, &randnum, sizeof(randnum));
    close(randgen);
    char *tmpdir = getenv("XDG_RUNTIME_DIR");
    if (tmpdir == NULL) {
        tmpdir = getenv("TMPDIR");
    }
    if (tmpdir != NULL) {
        snprintf(source, PATH_MAX, "%s/%llu.c", tmpdir, randnum);
    } else {
        snprintf(source, PATH_MAX, "/tmp/%llu.c", randnum);
    }
    int f = open(source, O_WRONLY | O_CREAT | O_TRUNC, 0755);
    int oldstdout = dup(1);
    dup2(f, 1);
    close(f);
    printf("#include <stdio.h>\n");
    printf("#include <stdint.h>\n");
    printf("int\nmain(int argc, char *argv[])\n");
    printf("{\nint32_t x;\n");
    printf("scanf(\"%%d\", &x);\n");
    printf("uint64_t n = x;\n");
    printf("int32_t mod = %d;\n", mod);
    printf("n = ((n %% mod) * ((n %% mod + 1) %% mod) / 2) %% mod;\n");
    printf("printf(\"%%llu\\n\", n);\n");
    printf("return 0;\n}\n");
    fflush(stdout);
    dup2(oldstdout, 1);
    close(oldstdout);
    if (!fork()) {
        execlp("gcc", "gcc", source, "-o", argv[1], NULL);
        _exit(1);
    }
    wait(NULL);
    chmod(argv[1], 0751);

    wait(NULL);
    exit(0);
}
