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

#define PATH_SIZE 40
#define BUFFER_SIZE 100

void readFile(const char *filename, int size, char path[], int option)
{
    FILE *fp, *src, *dst;
    char line[size];
    char c;

    char *token;
    char *exam_model;
    char model_path[PATH_SIZE] = "../examModels/MODELO";

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), fp))
    {
        switch (option)
        {
        case 0:
            token = strtok(line, " ");
            char first_path[PATH_SIZE];
            strcpy(first_path, path);
            strcat(first_path, token);
            mkdir(first_path, 0777);
            
        case 1:
            token = strtok(line, " ");
            exam_model = strtok(NULL, " ");
            // printf("%s ", exam_model);
            if (strcmp(exam_model, "A") == 0)
            {
                char first_path[PATH_SIZE];
                char second_path[PATH_SIZE];
                strcpy(second_path, model_path);
                strcat(second_path, "A.pdf");
                src = fopen(second_path, "rb");
                strcpy(first_path, path);
                strcat(first_path, token);
                // printf("%s\n", first_path);
                strcat(first_path, "/MODELOA.pdf");
                dst = fopen(first_path, "wb");

                while ((c = fgetc(src)) != EOF)
                {
                    fputc(c, dst);
                }

                
            }
            else if (strcmp(exam_model, "B") == 0)
            {
                char second_path[PATH_SIZE];
                strcpy(second_path, model_path);
                strcat(second_path, "B.pdf");

                src = fopen(second_path, "rb");
                strcpy(first_path, path);
                strcat(first_path, token);
                // printf("%s\n", first_path);
                strcat(first_path, "/MODELOB.pdf");
                dst = fopen(first_path, "wb");

                while ((c = fgetc(src)) != EOF)
                {
                    fputc(c, dst);
                }

             
            }
            else if (strcmp(exam_model, "C") == 0)
            {
                char second_path[PATH_SIZE];
                strcpy(second_path, model_path);
                strcat(second_path, "C.pdf");

                src = fopen(second_path, "rb");
                strcpy(first_path, path);
                strcat(first_path, token);
                // printf("%s\n", first_path);
                strcat(first_path, "/MODELOC.pdf");
                dst = fopen(first_path, "wb");

                while ((c = fgetc(src)) != EOF)
                {
                    fputc(c, dst);
                }
            }
        }
    }

    fclose(src);
    fclose(dst);
    fclose(fp);
}

void parse_args(int argc)
{
    if (argc != 3)
    {
        fprintf(stderr, "[Process] Usage: <file_name> <size of buffer> <path_to_create_directories>\n");
        exit(EXIT_FAILURE);
    }
}
