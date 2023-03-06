/************************************************************************************
 * Project: Practise 1 - Operating Systems                                          *
 * Program name: pd.c                                                               *
 * Author: Héctor Alberca Sánchez-Quintanar                                         *
 * Date: 05/02/2023                                                                 *
 * Purpose: Remove every file related to Students                                   *
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
#include <dirent.h>

#define DIRE "students"
#define TXT_FILE "Students.txt"
#define MAX_SIZE 4096

void remove_files(const char *p_dir_path);

/**
 * The main function.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 *
 * @returns EXIT_SUCCESS if the program runs successfully.
 */
int main(int argc, char const *argv[])
{
    /*Elimina todos los archivos en el directorio students*/
    remove_files(DIRE);
    return EXIT_SUCCESS;
}

/**
 * Removes all files in the directory DIRE.
 *
 * @returns None
 */
void remove_files(const char *p_dir_path)
{

    DIR *p_directory = opendir(p_dir_path);
    struct stat st;
    struct dirent *p_entry;

    while ((p_entry = readdir(p_directory)) != NULL)
    {
        /*Salta los directorios "." y ".."*/ 
        if (strcmp(p_entry->d_name, ".") == 0 || strcmp(p_entry->d_name, "..") == 0)
        {
            continue;
        }

        /* Si un archivo se llama Students.txt no se eliminea*/
        if (strcmp(p_entry->d_name, TXT_FILE) == 0)
        {
            continue;
        }
        /*Crea la ruta completa del archivo/directorio*/
        char file_path[MAX_SIZE];
        snprintf(file_path, sizeof(file_path), "%s/%s", p_dir_path, p_entry->d_name);

        if (stat(file_path, &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                /*Si es un directorio, llama a la función de nuevo*/
                remove_files(file_path);
            }
            else
            {
                // Si es un archivo, elimínalo
                if (remove(file_path) != 0)
                {
                    fprintf(stderr, "[PD] Couldn't remove file %s\n", file_path);
                }
            }
        }
        else
        {
            fprintf(stderr, "[PD] Couldn't stat file %s\n", file_path);
        }
    }

    closedir(p_directory);

    if (strcmp(p_dir_path, DIRE) != 0)
    {

        if (rmdir(p_dir_path) != 0)
        {
            fprintf(stderr, "[PD] Couldn't remove directory %s\n", p_dir_path);
        }
    }
}
