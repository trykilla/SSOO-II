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

    /*Defino las variables principales del programa (pipes, descriptores...)*/
    int pc_pipe[2];
    FILE *log_fp;

    char pipe_buf[MAX];
    char write_pipe[MAX];

    pipe(pc_pipe);
    sprintf(write_pipe, "%d", pc_pipe[1]);

    /*Defino e inicializo los argumentos que pasaremos al crear procesos*/
    char *p_args[] = {argv[1], argv[2], argv[3], NULL};
    char *pc_args[] = {argv[1], argv[2], argv[3], write_pipe, NULL};

    install_signal();

    /*Creo los procesos*/
    g_pids[0] = create_process(PATH_A, p_args);

    waitpid(g_pids[0], NULL, 0);
    g_pids[0] = 0;

    log_fp = open_single_file(FILES_PATH, 2);

    fprintf(log_fp, "******** System Log ********\nDirectory creation finished.\n");
    fflush(log_fp);

    sleep(1);

    g_pids[1] = create_process(PATH_B, p_args);
    g_pids[2] = create_process(PATH_C, pc_args);

    waitpid(g_pids[1], NULL, 0);
    g_pids[1] = 0;
    fprintf(log_fp, "Exam models copy finished.\n");

    /*Cierro el descriptor de escritura del pipe porque solo voy a leer*/
    close(pc_pipe[1]);
    read(pc_pipe[0], pipe_buf, sizeof(pipe_buf));

    waitpid(g_pids[2], NULL, 0);
    g_pids[2] = 0;
    fprintf(log_fp, "Files creation with mark necessary to reach media, finished.\n");
    fprintf(log_fp, "Media of the class: %s\nEND OF PROGRAM", pipe_buf);

    printf("\n[MANAGER] All child processes finished, the execution will be finished...\n");
    return EXIT_SUCCESS;
}



/**
 * Handles the SIGINT signal.
 *
 * @returns None
 */
void install_signal()
{

    if (signal(SIGINT, signal_handler) == SIG_ERR)
    {
        fprintf(stderr, "[MANAGER] Error setting signal handler");
        exit(EXIT_FAILURE);
    }
}

/**
 * Handles the SIGINT signal.
 *
 * @param sig The signal number.
 *
 * @returns None
 */
void signal_handler(int sig)
{

    if (sig == SIGINT)
    {
        // Abre un fichero o creálo si no está creado 
        printf("[MANAGER] SIGINT signal received (CTRL + C), the execution will be finished...\n");
        sleep(1);
        end_process();
        FILE *log_fp = fopen(FILES_PATH, "w");
        fprintf(log_fp, "******** System Log ********\n[MANAGER] User has interrupted the execution of the program.\n");

        create_process(PATH_D, NULL);

        exit(EXIT_SUCCESS);
    }
}

/**
 * Kills all child processes.
 *
 * @returns None
 */
void end_process(void)
{

    /*Comprobamos si hay procesos inicializados, si es así, los matamos.*/
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
