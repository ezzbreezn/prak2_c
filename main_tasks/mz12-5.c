#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

enum
{
    MASK = 1 << (CHAR_BIT - 1)
};

int 
main(int argc, char *argv[])
{
    int pid1, pid2;
    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s, SIGUSR1);
    sigaddset(&s, SIGUSR2);
    sigaddset(&s, SIGALRM);
    sigaddset(&s, SIGIO);
    sigprocmask(SIG_BLOCK, &s, NULL);
    int fd = signalfd(-1, &s, 0);
    pid1 = fork();
    if (!pid1) {
        struct signalfd_siginfo temp;
        unsigned char c = 0;
        int count = 0;
        while (1) {
            read(fd, &temp, sizeof(temp));
            if (temp.ssi_signo == SIGIO) {
                break;
            }
            if (temp.ssi_signo == SIGUSR2) {
                c |= 1;
                ++count;
            } else if (temp.ssi_signo == SIGUSR1) {
                ++count;
            } else {
                continue;
            }
            if (count == CHAR_BIT) {
                putchar(c);
                c = 0;
                count = 0;
            } else {
                c <<= 1;
            }
            int pid_rec = temp.ssi_pid; 
            kill(pid_rec, SIGALRM);
        }
        close(fd);
        exit(0);
    }
    pid2 = fork();
    if (!pid2) {
        struct signalfd_siginfo temp;
        int f = open(argv[1], O_RDONLY);
        unsigned char c;
        while (read(f, &c, sizeof(c)) == sizeof(c)) {
            for (int i = 0; i < CHAR_BIT; ++i) {
                if (c & MASK) {
                    kill(pid1, SIGUSR2);
                } else {
                    kill(pid1, SIGUSR1);
                }
                read(fd, &temp, sizeof(temp));
                while (temp.ssi_signo != SIGALRM) {
                    read(fd, &temp, sizeof(temp));
                }
                c <<= 1;
            }
        }
        close(f);
        kill(pid1, SIGIO);
        close(fd);
        exit(0);
    }
    wait(NULL);
    wait(NULL);
    close(fd);
    exit(0);
}
