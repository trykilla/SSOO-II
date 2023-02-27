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

#define CHILD_NUM 4
#define MAX 100

void signal_handler(int sig);
void end_process(void);
pid_t create_process(char path[], char *arg[]);
void install_signal(void);
void parse_args(int argc);

pid_t g_pids[CHILD_NUM];

int main(int argc, char *argv[])
{
    int fd1[2];
    FILE *fp;
    
    char *args[] = {argv[1], argv[2], argv[3], NULL};

    char buf[30];
    char wr_fd[MAX];

    pipe(fd1);

    sprintf(wr_fd, "%d", fd1[1]);

    char *pc_args[] = {argv[1], argv[2], argv[3], wr_fd, NULL};

    install_signal();

    g_pids[0] = create_process("exec/pa", args);

    waitpid(g_pids[0], NULL, 0);

    if ((fp = fopen("files/log.txt", "w+")) == NULL)
    {
        fprintf(stderr, "Error creating file.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "******** System Log ********\nDirectory creation finished.\n");

    // printf("[MANAGER] Child process A finished, the execution continues...\n");
    sleep(1);

    g_pids[1] = create_process("exec/pb", args);
    g_pids[2] = create_process("exec/pc", pc_args);
    waitpid(g_pids[1], NULL, 0);
    fprintf(fp, "Exam models copy finished.\n");

    printf("[MANAGER] Child process B finished, the execution continues...\n");
    //leeme de la tubería que le pasé al hijo:
    read(fd1[0], buf, sizeof(buf));
    

    waitpid(g_pids[2], NULL, 0);
    fprintf(fp, "Files creation with mark necessary to reach media, finished.\n");
    fprintf(fp, "Media of the class: %s\nEND OF PROGRAM", buf);


    // printf("[MANAGER] All child processes finished, the execution will be finished...\n");

    return EXIT_SUCCESS;
}

pid_t create_process(char path[], char *arg[])
{
    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, "[MANAGER] Error executing fork()\n");
        end_process();
        break;

    case 0:

        if (execv(path, arg) == -1)
        {
            fprintf(stderr, "[MANAGER] Error executing execv\n");
            exit(EXIT_FAILURE);
        }
    }
    return pid;
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
        if (g_pids[i] != 0)
        {
            if (kill(g_pids[i], SIGINT) == -1)
            {
                fprintf(stderr, "[MANAGER] Error killing process %d with pid %d \n", i, g_pids[i]);
            }
        }
    }
}
