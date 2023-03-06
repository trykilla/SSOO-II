/************************************************************************************
 * Project: Practise 1 - Operating Systems                                          *
 * Program name: pb.c                                                               *
 * Author: Héctor Alberca Sánchez-Quintanar                                         *
 * Date: 20/02/2023                                                                 *
 * Purpose: Copy exam models into students directories                              *
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
#include <def.h>

#define MAX_SIZE 100

int main(int argc, char const *argv[])
{
    /*Inicializamos las variables principales*/
    FILE *p_students_file;

    char path[MAX_SIZE];
    char line[MAX_SIZE];

    char *p_dni;
    char *p_exam_model;

    int fd1_src, fd2_src;
    char model_path[] = MODEL_PATH;

    /*Controlamos los argumentos*/
    parse_args(argc);
    strcpy(path, argv[2]);

    p_students_file = open_single_file(argv[0],1);

    /*Manipulamos el fichero, abriendo los directorios y copiando los modelos en los respectivos directorios del estudiante.*/
    while (fgets(line, sizeof(line), p_students_file))
    {

       
        p_dni = strtok(line, " ");
        p_exam_model = strtok(NULL, " ");

        if (strcmp(p_exam_model, MODEL_A) == 0)
        {
            char complete_exam_model[] = MODEL_A_PDF;
            open_files(&fd1_src, &fd2_src, model_path, path, p_dni, complete_exam_model);
            cp_file(fd1_src, fd2_src);
        }
        else if (strcmp(p_exam_model, MODEL_B) == 0)
        {
            char complete_exam_model[] = MODEL_B_PDF;
            open_files(&fd1_src, &fd2_src, model_path, path, p_dni, complete_exam_model);
            cp_file(fd1_src, fd2_src);
        }
        else if (strcmp(p_exam_model, MODEL_C) == 0)
        {
            char complete_exam_model[] = MODEL_C_PDF;
            open_files(&fd1_src, &fd2_src, model_path, path, p_dni, complete_exam_model);
            cp_file(fd1_src, fd2_src);
        }
    }
    close(fd1_src);
    close(fd2_src);
    fclose(p_students_file);
    return EXIT_SUCCESS;
}
