/************************************************************************************
 * Project: Practise 1 - Operating Systems                                          *
 * Program name: backup.c                                                           *
 * Author: Héctor Alberca Sánchez-Quintanar                                         *
 * Date: 05/02/2023                                                                 *
 * Purpose: Create backup of practice files...                                      *
 * Revision history: Héctor Alberca Sánchez-Quintanar, 20/02/2023                   *                                               *
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
#include <dirent.h>

void handler(int signum);
void cp_file(char *src, char *dst);
void cp_dir(char *src, char *dst);

#define SRC "../p1"
#define DST "../practice_backup"
#define BUFF_SIZE 1024
#define ALARM_TIME 60

/**
 * The main function for the backup process.
 *
 * @param argc The number of arguments.
 * @param argv The arguments.
 *
 * @returns EXIT_SUCCESS if the process ran successfully.
 */
int main(int argc, char const *argv[])
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handler;
    sigaction(SIGALRM, &act, NULL);

    printf("[BACKUP] STARTING BACKUP PROCESS\nTO FINISH PROCESS, KILL IT WITH (KILL -9)\n");
    alarm(ALARM_TIME);
    while (1);

    return EXIT_SUCCESS;
}

/**
 * Copies files from src to dst.
 *
 * @param src The source directory.
 * @param dst The destination directory.
 *
 * @returns None
 */
void handler(int signum)
{
    printf("[BACKUP] Copying files...\n");
    char *src = SRC;
    char *dst = DST;
    cp_dir(src, dst);

    alarm(ALARM_TIME);
}

/**
 * Copies a file from src to dst.
 *
 * @param src The source file.
 * @param dst The destination file.
 *
 * @returns None
 */
void cp_file(char *src, char *dst)
{
    int fd_src, fd_dst;
    char buf[BUFF_SIZE];
    ssize_t bytes_read;

    fd_src = open(src, O_RDONLY);
    if (fd_src == -1)
    {
        fprintf(stderr,"[BACKUP] Error opening src file");
        exit(EXIT_FAILURE);
    }

    fd_dst = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd_dst == -1)
    {
        fprintf(stderr,"[BACKUP] Error opening dst file");
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(fd_src, buf, sizeof(buf))) > 0)
    {
        if (write(fd_dst, buf, bytes_read) != bytes_read)
        {
            fprintf(stderr,"[BACKUP] Error writing to dst file");
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1)
    {
        fprintf(stderr,"[BACKUP] Error reading from src file");
        exit(EXIT_FAILURE);
    }

    if (close(fd_src) == -1)
    {
        fprintf(stderr,"[BACKUP] Error closing src file");
        exit(EXIT_FAILURE);
    }

    if (close(fd_dst) == -1)
    {
        fprintf(stder,,"[BACKUP] Error closing dst file");
        exit(EXIT_FAILURE);
    }
}

/**
 * Copies a directory from src to dst.
 *
 * @param src The source directory.
 * @param dst The destination directory.
 *
 * @returns None
 */
void cp_dir(char *src, char *dst)
{
    DIR *p_directory;
    struct dirent *entry;
    struct stat st;
    char src_path[BUFF_SIZE], dst_path[BUFF_SIZE];

    // Crear directorio de destino si no existe
    if (mkdir(dst, 0777) == -1 && errno != EEXIST)
    {
        fprintf(stderr,"[BACKUP] Could not create dst directory");
        exit(EXIT_FAILURE);
    }

    // Abrir directorio de origen
    p_directory = opendir(src);
    if (!p_directory)
    {
        fprintf(stderr,"[BACKUP] Could not open src directory");
        exit(EXIT_FAILURE);
    }

    // Copiar cada entrada en el directorio
    while ((entry = readdir(p_directory)))
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue; // Ignorar entradas "." y ".."
        }

        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dst, entry->d_name);

        // Obtener información de archivo/directorio
        if (lstat(src_path, &st) == -1)
        {
            fprintf(stderr,"[BACKUP] Error getting file info");
            exit(EXIT_FAILURE);
        }

        // Copiar archivo o directorio
        if (S_ISREG(st.st_mode))
        {
            // Copiar archivo regular
            cp_file(src_path, dst_path);
        }
        else if (S_ISDIR(st.st_mode))
        {
            // Copiar directorio
            cp_dir(src_path, dst_path);
        }
    }

    // Cerrar directorio
    if (closedir(p_directory) == -1)
    {
        fprintf(stderr,"[BACKUP] Error closing src directory");
        exit(EXIT_FAILURE);
    }
}
