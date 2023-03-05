/************************************************************************************
 * Project: Practise 1 - Operating Systems                                          *
 * Program name: manager.c                                                          *
 * Author: Héctor Alberca Sánchez-Quintanar                                         *
 * Date: 20/02/2023                                                                 *
 * Purpose: Process managing                                                        *
 * Revision history: Héctor Alberca Sánchez-Quintanar, 20/02/2023                   *
 * Create of process A, B, C, D                                                     *
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
#include <my_lib.h>
#include <def.h>


#define CHILD_NUM 4
#define MAX 100

void signal_handler(int sig);
void end_process(void);
void install_signal(void);
void parse_args(int argc);

pid_t g_pids[CHILD_NUM];


int main(int argc, char *argv[])
{
    
    int fd1[2];
    FILE *fp;

    char *args[] = {argv[1], argv[2], argv[3], NULL};

    char buf[MAX];
    char wr_fd[MAX];

    pipe(fd1);

    sprintf(wr_fd, "%d", fd1[1]);

    char *pc_args[] = {argv[1], argv[2], argv[3], wr_fd, NULL};

    install_signal();

    g_pids[0] = create_process(PATH_A, args);

    waitpid(g_pids[0], NULL, 0);
    g_pids[0] = 0;

    fp = open_single_file(FILES_PATH, 2);
    
  
    fprintf(fp, "******** System Log ********\nDirectory creation finished.\n");
    fflush(fp);

    sleep(1);

    g_pids[1] = create_process(PATH_B, args);
    g_pids[2] = create_process(PATH_C, pc_args);
    
    waitpid(g_pids[1], NULL, 0);
    g_pids[1] = 0;
    fprintf(fp, "Exam models copy finished.\n");

    // leeme de la tubería que le pasé al hijo:
    // vamos a cerrar la parte de la tubería que no vamos a usar
    close(fd1[1]);
    read(fd1[0], buf, sizeof(buf));

    waitpid(g_pids[2], NULL, 0);
    g_pids[2] = 0;
    fprintf(fp, "Files creation with mark necessary to reach media, finished.\n");
    fprintf(fp, "Media of the class: %s\nEND OF PROGRAM", buf);

    
    printf("\n[MANAGER] All child processes finished, the execution will be finished...\n");
    return EXIT_SUCCESS;
}



void install_signal()
{


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
        //Abre un fichero o crealo si no está creado y escribe [MANAGER] User has interrupted the execution of the program.\n"
        printf("[MANAGER] SIGINT signal received (CTRL + C), the execution will be finished...\n");
        sleep(1);
        end_process();
        FILE *fp = fopen(FILES_PATH, "w");
        fprintf(fp,"******** System Log ********\n[MANAGER] User has interrupted the execution of the program.\n");
        
        create_process(PATH_D, NULL);
        
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
