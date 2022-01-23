#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/mman.h>

enum
{
    ARGNUM = 3
};

int
main(int argc, char *argv[])
{
    if (argc <= ARGNUM) {
        fprintf(stderr, "wrong input\n");
        exit(1);
    }
    int nproc, maxval;
    key_t key;
    sscanf(argv[1], "%d", &nproc);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &maxval);

    int sem_id = semget(key, nproc, IPC_CREAT | 0600);
    unsigned short *init_val = realloc(NULL, nproc * sizeof(*init_val));
    semctl(sem_id, nproc, SETALL, init_val);
    free(init_val);

    int *ptr = mmap(NULL, 2 * sizeof(*ptr), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    struct sembuf up = {}, down = {};
    up.sem_op = 1;
    down.sem_op = -1;
    int val, from = -1;

    for (int i = 0; i < nproc; ++i) {
        int pid = fork();
        if (!pid) {
            down.sem_num = i;
            while (1) {
                semop(sem_id, &down, 1);
                val = ptr[0];
                from = ptr[1];
                if (from == -2) {
                    exit(0);
                }
                printf("%d %d %d\n", i + 1, val, from + 1);
                fflush(stdout);
                ++val;
                if (val > maxval) {
                    ptr[1] = -2;
                    for (int j = 0; j < nproc; ++j) {
                        up.sem_num = j;
                        semop(sem_id, &up, 1);
                    }
                    exit(0);
                }
                ptr[0] = val;
                ptr[1] = i;
                val %= nproc;
                val *= val;
                val %= nproc;
                val *= val;
                val %= nproc;
                up.sem_num = val;
                semop(sem_id, &up, 1);

            }
        }
    }
    ptr[0] = 0;
    ptr[1] = -1;
    up.sem_num = 0;
    semop(sem_id, &up, 1);
    for(int i = 0; i < nproc; ++i) {
        wait(NULL);
    }
    semctl(sem_id, 0, IPC_RMID, NULL);
    munmap(ptr, 2 * sizeof(*ptr));
    exit(0);
}
