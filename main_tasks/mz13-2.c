#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/mman.h>

enum
{
    FST_ARGS = 5,
    MAX_NUM = 10
};



int
main(int argc, char *argv[])
{
    if (argc < FST_ARGS) {
        fprintf(stderr, "wrong input\n");
        exit(1);
    }
    int count, nproc, iter_count;
    key_t key;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &nproc);
    sscanf(argv[4], "%d", &iter_count);

    int *ptr = mmap(NULL, count * sizeof(*ptr), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (int i = 0; i < count; ++i) {
        scanf("%d", &ptr[i]);
    }

    unsigned short *sem_init = realloc(NULL, count * sizeof(*sem_init));
    for (int i = 0; i < count; ++i) {
        sem_init[i] = 1;
    }
    int sem_id = semget(key, count, IPC_CREAT | 0600);
    semctl(sem_id, count, SETALL, sem_init);
    free(sem_init);
    for (int i = 0; i < nproc; ++i) {
        int pid = fork();
        if (!pid) {
            struct sembuf up[2] = {}, down[2] = {};
            up[0].sem_op = 1;
            up[1].sem_op = 1;
            down[0].sem_op = -1;
            down[1].sem_op = -1;
            unsigned int seed;
            sscanf(argv[FST_ARGS + i], "%u", &seed);
            srand(seed);
            for (int j = 0; j < iter_count; ++j) {
                int ind1 = rand() % count;
                int ind2 = rand() % count;
                int val = rand() % MAX_NUM;
                up[0].sem_num = ind1;
                up[1].sem_num = ind2;
                down[0].sem_num = ind1;
                down[1].sem_num = ind2;
                if (ind1 == ind2) {
                    semop(sem_id, &down[0], 1);
                    operation(ptr, ind1, ind2, val);
                    semop(sem_id, &up[0], 1);
                } else {
                    semop(sem_id, down, 2);
                    operation(ptr, ind1, ind2, val);
                    semop(sem_id, up, 2);
                }
            }
            exit(0);
        }
    }
    for (int i = 0; i < nproc; ++i) {
        wait(NULL);
    }
    for (int i = 0; i < count; ++i) {
        printf("%d\n", ptr[i]);
        fflush(stdout);
    }
    munmap(ptr, count * sizeof(*ptr));
    semctl(sem_id, count, IPC_RMID, NULL);
    exit(0);
}
