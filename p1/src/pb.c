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

#define MAX_SIZE 100

int main(int argc, char const *argv[])
{
    FILE *fp;

    char path[MAX_SIZE];
    char line[MAX_SIZE];

    char *token;
    char *exam_model;

    int fd1, fd2;
    char model_path[] = "examModels/";

    parse_args(argc);
    strcpy(path, argv[2]);

    fp = open_single_file(argv[0],1);

    while (fgets(line, sizeof(line), fp))
    {

       
        token = strtok(line, " ");
        exam_model = strtok(NULL, " ");

        if (strcmp(exam_model, "A") == 0)
        {
            char complete_exam_model[] = "/MODELOA.pdf";
            open_files(&fd1, &fd2, model_path, path, token, complete_exam_model);
            cp_file(fd1, fd2);
        }
        else if (strcmp(exam_model, "B") == 0)
        {
            char complete_exam_model[] = "/MODELOB.pdf";
            open_files(&fd1, &fd2, model_path, path, token, complete_exam_model);
            cp_file(fd1, fd2);
        }
        else if (strcmp(exam_model, "C") == 0)
        {
            char complete_exam_model[] = "/MODELOC.pdf";
            open_files(&fd1, &fd2, model_path, path, token, complete_exam_model);
            cp_file(fd1, fd2);
        }
    }
    close(fd1);
    close(fd2);
    fclose(fp);
    return EXIT_SUCCESS;
}
