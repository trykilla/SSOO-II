/************************************************************************************
 * Project: Practise 1 - Operating Systems                                          *
 * Program name: pc.c                                                               *
 * Author: Héctor Alberca Sánchez-Quintanar                                         *
 * Date: 20/02/2023                                                                 *
 * Purpose: Calculates the media mark that one student need to pass the subject.    *
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
#include <my_lib.h>
#include <def.h>

#define MAX_SIZE 4096
#define MARK_SIZE 10

void parse_args_pc(int argc);

int main(int argc, char const *argv[])
{
    /*Definimos las variables principales*/
    FILE *p_student_fp;
    FILE *f_mark;
    char path[MAX_SIZE];
    char *p_dni;

    int mark;
    int option = 1;
    float class_media = 0;
    float child_counter = 0;

    /*Comprobamos los parámetros*/
    parse_args_pc(argc);
    strcpy(path, argv[2]);

    char line[MAX_SIZE];
    p_student_fp = open_single_file(argv[0], option);

    while (fgets(line, sizeof(line), p_student_fp))
    {
        int needed_mark;
        /*Obtenemos el parámetro que necesitamos, la nota */
        char msg[] = PC_MSG;
        char tmp_path[PATH_SIZE];
        p_dni = strtok(line, " ");
        strtok(NULL, " ");
        mark = atoi(strtok(NULL, " "));
        class_media += mark;
        /*Calculamos las medias y contamos los estudiantes*/
        child_counter++;
        needed_mark = 2 * MIN_GRADE - mark;

        strcpy(tmp_path, path);
        strcat(tmp_path, p_dni);

        strcat(tmp_path, NEED_MARK);
        f_mark = open_single_file(tmp_path, 2);

        fprintf(f_mark, "%s %d", msg, needed_mark);
    }

    /*Calculamos la media de la clase y convertimos mediante gcvt*/
    class_media = class_media / child_counter;
    char class_media_char[MARK_SIZE];
    gcvt(class_media, sizeof(class_media_char), class_media_char);
    write(atoi(argv[3]), class_media_char, sizeof(class_media_char));

    fclose(p_student_fp);
    return EXIT_SUCCESS;
}

void parse_args_pc(int argc)
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage:<file> <path> <pipe>\n");
        exit(EXIT_FAILURE);
    }
}
