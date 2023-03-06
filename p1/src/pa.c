/************************************************************************************
 * Project: Practise 1 - Operating Systems                                          *
 * Program name: pa.c                                                               *
 * Author: Héctor Alberca Sánchez-Quintanar                                         *
 * Date: 20/02/2023                                                                 *
 * Purpose: Create directories for all the students                                 *
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
#include <my_lib.h>

#define MAX_SIZE 100

int main(int argc, char const *argv[])
{

    /*Definimos las variables principales*/
    FILE *p_student_fp;
    char *p_dni;
    char path[MAX_SIZE];
    char line[MAX_SIZE];
    int option = 1;

    /*Comprobamos que el número de argumentos es correcto*/
    parse_args(argc);
    /*Almacenamos el contenido de la línea de comandos en el path*/
    strcpy(path, argv[2]);
    p_student_fp = open_single_file(argv[0],option);

    /*Manipulamos el fichero, registrando el dni y creando las carpetas.*/
    while (fgets(line, sizeof(line), p_student_fp))
    {

        p_dni = strtok(line, " ");
        char tmp_path[PATH_SIZE];
        strcpy(tmp_path, path);
        strcat(tmp_path, p_dni);
        mkdir(tmp_path, 0777);
    }
    fclose(p_student_fp);
    return EXIT_SUCCESS;
}
