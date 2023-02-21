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

#define PATH_SIZE 20

int readFile(const char *filename, int size, char path[])
{
    FILE *fp;
    char line[size];

    char *token;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    while (fgets(line, sizeof(line), fp))
    {
        token = strtok(line, " ");
        char firstPath[PATH_SIZE];
        strcpy(firstPath, path);
        strcat(firstPath, token);
        mkdir(firstPath, 0777);
    }

    fclose(fp);
}

