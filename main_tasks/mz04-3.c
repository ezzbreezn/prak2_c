#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

enum
{
    START_SIZE = 100
};

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void
print(struct Node *arr, int32_t index)
{
    if (arr[index].right_idx != 0) {
        print(arr, arr[index].right_idx);
    }
    printf("%d\n", arr[index].key);
    if (arr[index].left_idx != 0) {
        print(arr, arr[index].left_idx);
    }
}

int
main(int argc, char *argv[])
{
    unsigned long long size = START_SIZE, real_size = 0, rb;
    struct Node *arr = realloc(NULL, size * sizeof(struct Node));
    if (arr == NULL) {
        fprintf(stderr, "allocation error\n");
        return 1;
    }
    int f = open(argv[1], O_RDONLY);
    if (f == -1) {
        fprintf(stderr, "read error\n");
        return 1;
    }
    while ((rb = read(f, &arr[real_size], sizeof(struct Node))) > 0) {
        if (real_size + 1 > size) {
            size *= 2;
            arr = realloc(arr, size * sizeof(struct Node));
        }
        ++real_size;
    }
    if (rb == -1) {
        fprintf(stderr, "read error\n");
        return 1;
    }
    print(arr, 0);
    free(arr);
    return 0;
}
