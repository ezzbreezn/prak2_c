#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "read error\n");
        return 1;
    }
    off_t min_offset = 0, temp_offset = 0;
    ssize_t read_nums = 0;
    long long min = 0, temp = 0;
    read_nums = read(fd, &min, sizeof(min));
    if (read_nums != sizeof(min)) {
        fprintf(stderr, "read error\n");
        return 0;
    }
    temp_offset += read_nums;
    while ((read_nums = read(fd, &temp, sizeof(temp))) == sizeof(temp)) {
        if (temp < min) {
            min = temp;
            min_offset = temp_offset;
        }
        temp_offset += read_nums;
        temp = 0;
    }
    unsigned long long ans = (unsigned long long) min;
    ans = -ans;
    if (lseek(fd, min_offset, SEEK_SET) == -1) {
        fprintf(stderr, "lseek error\n");
        return 1;
    }
    if (write(fd, &ans, sizeof(ans)) != sizeof(ans)) {
        fprintf(stderr, "write error\n");
        return 1;
    }
    close(fd);
    return 0;
}

