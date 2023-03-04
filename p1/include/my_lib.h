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
FILE *open_single_file(const char file_name[], int option);

FILE *open_single_file(const char file_name[], int option)
{
    FILE *fp;

    switch (option)
    {
    case 1:
        fp = fopen(file_name, "r");
        if (fp == NULL)
        {
            fprintf(stderr, "Error opening file.\n");
            exit(EXIT_FAILURE);
        }
        break;
    case 2:
        fp = fopen(file_name, "w+");
        if (fp == NULL)
        {
            fprintf(stderr, "Error opening file.\n");
            exit(EXIT_FAILURE);
        }
        break;
    }

    return fp;
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
