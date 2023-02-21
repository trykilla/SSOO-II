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
void create_process(char path[], char *arg[]);
void install_signal(void);
void parse_args(int argc);
#define CHILD_NUM 4

pid_t pids[CHILD_NUM];

int main(int argc, char *argv[])
{
    char *args[] = {argv[1], argv[2], argv[3], NULL};

    install_signal();
    create_process("./exec/pa", args);

    waitpid(pids[0], NULL, 0);
    

    return EXIT_SUCCESS;
}

void create_process(char path[], char *arg[])
{
    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, "[MANAGER] Error executing fork()\n");
        end_process();
        break;

    case 0:
        printf("%s, %s %s %s \n", path, arg[0], arg[1], arg[2]);

        printf("path: %s", path);
        if (execv(path, arg) == -1)
        {
            fprintf(stderr, "[MANAGER] Error executing execl\n");
            exit(EXIT_FAILURE);
        }
    }
}

void install_signal()
{
    if (signal(SIGUSR1, signal_handler) == SIG_ERR)
    {
        fprintf(stderr, "[MANAGER] Error setting signal handler");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, signal_handler) == SIG_ERR)
    {
        fprintf(stderr, "[MANAGER] Error setting signal handler");
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int sig)
{
    if (sig == SIGUSR1)
    {
        printf("[MANAGER] Child proccess finished, the execution continues...\n");
        sleep(1);
    }

    if (sig == SIGINT)
    {
        printf("[MANAGER] SIGINT signal received (CTRL + C), the execution will be finished...\n");
        end_process();
        exit(EXIT_SUCCESS);
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
