/************************************************************************************
 * Project: Practise 1 - Operating Systems                                          *                    
 * Program name: manager.c                                                          *                                 
 * Author: Héctor Alberca Sánchez-Quintanar                                         *
 * Date: 20/02/2023                                                                 *
 * Purpose: Process managing                                                        *
 * Revision history: Héctor Alberca Sánchez-Quintanar, 20/02/2023                   *
 * Create of process A and B                                                        *
 ************************************************************************************/

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
        fprintf(stderr, "[MANAGER] Error setting signal handler");
        exit(EXIT_FAILURE);
    }

    switch (pids[0] = fork())
    {
    case -1:
        fprintf(stderr, "[MANAGER] Error creating child process");
        end_process();
        break;

    case 0:
        char *args[] = {argv[1], argv[2], argv[3], NULL};

        execv("exec/pa", args);
        break;
    }

    if ((pid = wait(NULL)) == -1)
    {
        fprintf(stderr, "[MANAGER] Error waiting for child process\n");
        end_process();
    }

    if (pid == pids[0])
    {
        printf("[MANAGER] Child process %d finished\n", pid);
    }



    return EXIT_SUCCESS;
}

void signal_handler(int sig)
{
    if (sig == SIGUSR1)
    {
        printf("[MANAGER] Child proccess finished, the execution continues...\n");
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
                fprintf(stderr, "[MANAGER] Error killing child process\n");
            }
        }
    }
}
