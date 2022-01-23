#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

enum
{
    MAXSIZE = 100
};

struct Msg
{
    long mtype;
    char mtext[MAXSIZE];
};

int
main(int argc, char *argv[])
{
    if (argc <= 5) {
        fprintf(stderr, "wrong input\n");
        exit(1);
    }
    key_t key;
    unsigned long long n;
    int64_t value1, value2, maxval;
    sscanf(argv[1], "%d", &key);
    sscanf(argv[2], "%llu", &n);
    sscanf(argv[3], "%lld", &value1);
    sscanf(argv[4], "%lld", &value2);
    sscanf(argv[5], "%lld", &maxval);
    
    int msq_id = msgget(key, IPC_CREAT | 0600);
    for (unsigned long long i = 0; i < n; ++i) {
        int pid = fork();
        if (pid == -1) {
            msgctl(msq_id, IPC_RMID, 0);
            for (unsigned long long j = 0; j < n; ++j) {
                wait(NULL);
            }
            exit(1);
        }
        if (!pid) {
            struct Msg temp = {};
            unsigned long long proc_no = i + 1;
            while (1) {
                errno = 0;
                if (msgrcv(msq_id, &temp, MAXSIZE, proc_no, 0) == -1) {
                    errno = 0;
                    exit(1);
                }
                sscanf(temp.mtext, "%lld%lld", &value1, &value2);
                value1 += value2;
                printf("%ld %lld\n", temp.mtype - 1, value1);
                fflush(stdout);
                if (value1 > maxval) {
                    msgctl(msq_id, IPC_RMID, 0);
                    exit(0);
                }
                snprintf(temp.mtext, MAXSIZE, "%lld %lld ", value2, value1);
                temp.mtype = (value1 % n) + 1;
                msgsnd(msq_id, &temp, MAXSIZE, 0);
            }
        }
    }
    struct Msg init = {};
    init.mtype = 1;
    snprintf(init.mtext, MAXSIZE, "%lld %lld ", value1, value2);
    msgsnd(msq_id, &init, MAXSIZE, 0);
    for (unsigned long long i = 0; i < n; ++i) {
        wait(NULL);
    }
    exit(0);
}
