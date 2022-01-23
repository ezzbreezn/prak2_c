#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

unsigned long long hash_function(unsigned long long);

void
write_seq(int fd, unsigned long long h, int count)
{
    if (count == 0) {
        return;
    }
    write_seq(fd, hash_function(h), count - 1);
    if (write(fd, &h, sizeof(h)) != sizeof(h)) {
        fprintf(stderr, "write error\n");
        exit(1);
    }
}

int
main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    unsigned long long h0;
    long long count;
    if (sscanf(argv[2], "%llx", &h0) < 1) {
        fprintf(stderr, "read error\n");
        return 1;
    }
    if (sscanf(argv[3], "%lld", &count) < 1) {
        fprintf(stderr, "read error\n");
        return 1;
    }
    if (count < 0) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    int fd = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, 0660);
    if (fd == -1) {
        fprintf(stderr, "open error\n");
        return 1;
    }
    write_seq(fd, h0, count);
    close(fd);
    return 0;
}   
