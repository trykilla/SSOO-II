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
#define MAX_SIZE 4096

void remove_files(const char *dir);

int main(int argc, char const *argv[])
{
    //elimina todos los archivos en el directorio students
    remove_files(DIRE);
    return EXIT_SUCCESS;
}

void remove_files(const char *dir)
{

    DIR *d = opendir(dir);
    struct stat st;

    struct dirent *entry;

    while ((entry = readdir(d)) != NULL)
    {
        // Salta los directorios "." y ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        //Si un archivo se llama Students.txt no lo elimines
        if (strcmp(entry->d_name, "Students.txt") == 0)
        {
            continue;
        }
        // Crea la ruta completa del archivo/directorio
        char file_path[MAX_SIZE];
        snprintf(file_path, sizeof(file_path), "%s/%s", dir, entry->d_name);

        if (stat(file_path, &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                // Si es un directorio, llama a la función de nuevo
                remove_files(file_path);
            }
            else
            {
                // Si es un archivo, elimínalo
                if (remove(file_path) != 0)
                {
                    fprintf(stderr,"Couldn't remove file %s\n", file_path);
                }
            }
        }
        else
        {
            fprintf(stderr,"Couldn't stat file %s\n", file_path);
        }
    }

    closedir(d);

    if (strcmp(dir, DIRE) != 0)
    {

        if (rmdir(dir) != 0)
        {
            fprintf(stderr,"Couldn't remove directory %s\n", dir);
        }
    }
}
