#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

void signal_handler(int sig);
void end_process(void);

#define CHILD_NUM 4

pid_t pids[CHILD_NUM];

int main(int argc, char *argv[])
{
    pid_t pid;

    if (signal(SIGUSR1, signal_handler) == SIG_ERR)
    {
        fprintf(stderr, "Error setting signal handler");
        exit(EXIT_FAILURE);
    }

    switch (pids[0] = fork())
    {
    case -1:
        fprintf(stderr, "Error creating child process");
        end_process();
        break;

    case 0:
        char *args[] = {argv[1], argv[2], argv[3], NULL};
        printf("Hello");
        execv("exec/pa", args);
        break;
    }

    pid = waitpid(pids[0], NULL, NULL);

    return EXIT_SUCCESS;
}




void signal_handler(int sig)
{
    if (sig == SIGUSR1)
    {
        printf("Child proccess finished, the execution continues...\n");
        sleep(1);
    }
}

void end_process(void)
{
    int i;
    for (i = 0; i < CHILD_NUM; i++)
    {
        if (pids[i] != 0)
        {
            if (kill(pids[i], SIGUSR1) == -1)
            {
                fprintf(stderr, "Error killing child process\n");
            }
        }
    }
}
