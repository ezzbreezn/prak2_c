#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void
print(int f, int32_t idx)
{
    if (lseek(f, idx * sizeof(struct Node), SEEK_SET) == -1) {
        fprintf(stderr, "lseek error\n");
        exit(1);
    }
    struct Node temp;
    if (read(f, &temp, sizeof(temp)) != sizeof(temp)) {
        fprintf(stderr, "read error\n");
        exit(1);
    }
    if (temp.right_idx != 0) {
        print(f, temp.right_idx);
    }
    printf("%d\n", temp.key);
    if (temp.left_idx != 0) {
        print(f, temp.left_idx);
    }
}

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    int f = open(argv[1], O_RDONLY);
    if (f == -1) {
        fprintf(stderr, "read error\n");
        return 1;
    }
    print(f, 0);
    return 0;
}

