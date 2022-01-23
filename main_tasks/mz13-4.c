#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdint.h>

int
main(int argc, char *argv[])
{
    setbuf(stdin, NULL);
    if (argc <= 1) {
        fprintf(stderr, "wrong input\n");
        exit(1);
    }
    int count;
    sscanf(argv[1], "%d", &count);
    key_t key = ftok(argv[0], 0);
    int sem_id = semget(key, count, IPC_CREAT | 0666);
    unsigned short *init = realloc(NULL, count * sizeof(*init));
    semctl(sem_id, count, SETALL, init);
    free(init);
    int32_t temp;
    struct sembuf down = {}, up = {};
    down.sem_op = -1;
    up.sem_op = 1;
    for (int i = 0; i < count; ++i) {
        int pid = fork();
        if (!pid) {
            down.sem_num = i;
            while (1) {
                if (semop(sem_id, &down, 1) == -1) {
                    exit(0);
                }
                if (scanf("%d", &temp) < 1) {
                    semctl(sem_id, 0, IPC_RMID, NULL);
                    exit(0);
                }
                printf("%d %d\n", i, temp);
                fflush(stdout);
                if (temp >= 0) {
                    temp %= count;
                } else {
                    int diff;
                    if (count >= 0) {
                        diff = count - 1;
                    } else {
                        diff = -count - 1;
                    }
                    temp -= ((temp - diff) / count) * count;
                }
                up.sem_num = temp;
                semop(sem_id, &up, 1);
            }
        }
    }
    up.sem_num = 0;
    semop(sem_id, &up, 1);
    for (int i = 0; i < count; ++i) {
        wait(NULL);
    }
    semctl(sem_id, 0, IPC_RMID, NULL);
    exit(0);
}   
