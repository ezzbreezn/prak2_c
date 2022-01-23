#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdlib.h>

enum
{
    OFFSET_FIELD = 9,
    MASK = 511
};


int
main(int argc, char *argv[])
{
    if (argc <= 2) {
        fprintf(stderr, "wrong input\n");
        return 1;
    }
    int f = open(argv[1], O_RDONLY);
    if (f == -1) {
        fprintf(stderr, "cant open the file\n");
        return 0;
    }
    uint16_t start;
    if (sscanf(argv[2], "%hx", &start) < 1) {
        fprintf(stderr, "sscanf error\n");
        return 1;
    }
    struct stat info;
    if (stat(argv[1], &info) == -1) {
        fprintf(stderr, "cant read file info\n");
        exit(0);
    }
    off_t size = info.st_size;
    if (!S_ISREG(info.st_mode) || size == 0) {
        fprintf(stderr, "cant map\n");
        exit(0);
    }
    unsigned char *mem = mmap(NULL, size, PROT_READ, MAP_PRIVATE, f, 0);
    if (mem == MAP_FAILED) {
        fprintf(stderr, "map failed\n");
        exit(1);
    }
    uint16_t addr, content, table_str, record, phys;
    close(f);
    while (scanf("%hx", &addr) >= 1) {
        table_str = addr >> OFFSET_FIELD;
        record = *(uint16_t *) (mem + start + 2 * table_str);
        phys = ((record >> OFFSET_FIELD) << OFFSET_FIELD) | (addr & MASK);
        content = *(uint16_t *) (mem + phys);
        printf("%hu\n", content);
    }
    munmap(mem, size);
    return 0;
}

