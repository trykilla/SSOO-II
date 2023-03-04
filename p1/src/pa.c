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

    FILE *fp;
    char path[MAX_SIZE];
    char line[MAX_SIZE];

    char *token;

    parse_args(argc);
    strcpy(path, argv[2]);
    printf("%s", argv[0]);

    fp = open_single_file(argv[0],1);

    
    while (fgets(line, sizeof(line), fp))
    {

        token = strtok(line, " ");
        char first_path[PATH_SIZE];
        strcpy(first_path, path);
        strcat(first_path, token);
        mkdir(first_path, 0777);
    }
    fclose(fp);
    return EXIT_SUCCESS;
}
