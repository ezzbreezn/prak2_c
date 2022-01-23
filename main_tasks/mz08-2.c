#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    if (fork()) {
        wait(NULL);
        printf("1\n");
    } else if (fork()) {
        wait(NULL);
        printf("2 ");
    } else {
        printf("3 ");
    }
    return 0;
}
