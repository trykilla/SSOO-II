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
#define BUFFER_SIZE 4096
#define MIN_GRADE 5

void parse_args(int argc);
void cp_file(int fd1, int fd2);
void open_files(int *fd1, int *fd2, char model_path[], char path[], char token[], char exam_model[]);
// void open_files(int fd1, int fd2, char model_path[], char path[], char token[], char complete_exam_model[]);

void readFile(const char *filename, int size, char path[], int option, char const pipe_name[])
{
    FILE *fp;
    FILE *f_mark;

    int fd1, fd2;
    int mark;
    float class_media = 0;
    float child_counter;

    char *exam_model;
    char *token;

    char line[size];
    char model_path[] = "examModels/";

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
            break;

        case 1:
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

            break;
        case 2:

            int needed_mark;

            char msg[] = "Mark you should obtain in this new exam to pass is:";

            token = strtok(line, " ");
            exam_model = strtok(NULL, " ");
            mark = atoi(strtok(NULL, " "));
            class_media += mark;
            
            child_counter++;
            needed_mark = 2 * MIN_GRADE - mark;

            strcpy(first_path, path);
            strcat(first_path, token);

            strcat(first_path, "/needed_mark.txt");

            if ((f_mark = fopen(first_path, "w+")) == NULL)
            {
                fprintf(stderr, "Error creating file.\n");
                exit(EXIT_FAILURE);
            }

            fprintf(f_mark, "%s %d", msg, needed_mark);
            break;
        }
    }

    

    fclose(fp);
    close(fd1);
    close(fd2);
    fclose(f_mark);
}

void open_files(int *fd1, int *fd2, char model_path[], char path[], char token[], char exam_model[])
{

    char first_path[PATH_SIZE];
    char second_path[PATH_SIZE];

    strcpy(second_path, model_path);
    strcat(second_path, exam_model);

    if ((*fd1 = open(second_path, O_RDONLY)) == -1)
    {
        fprintf(stderr, "Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(first_path, path);
    strcat(token, exam_model);
    strcat(first_path, token);

    if ((*fd2 = open(first_path, O_WRONLY | O_CREAT, 0777)) == -1)
    {
        fprintf(stderr, "Error opening file.\n");
        exit(EXIT_FAILURE);
    }
}

void parse_args(int argc)
{
    if (argc != 3)
    {
        fprintf(stderr, "[Process] Usage: <file_name> <size of buffer> <path_to_create_directories>\n");
        exit(EXIT_FAILURE);
    }
}

void cp_file(int fd1, int fd2)
{
    char buffer[BUFFER_SIZE];
    int read_bytes;

    while ((read_bytes = read(fd1, buffer, sizeof(buffer))) > 0)
    {
        if (write(fd2, buffer, read_bytes) != read_bytes)
        {
            fprintf(stderr, "Error writing file.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (read_bytes == -1)
    {
        fprintf(stderr, "Error reading file.\n");
        exit(EXIT_FAILURE);
    }
}
