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

#include "../include/my_lib.h"

#define MAX_SIZE 100

int main(int argc, char const *argv[])
{

    char path[MAX_SIZE];

    if (argc != 3)
    {
        fprintf(stderr, "[PA] Usage: <file_name> <size of buffer> <path_to_create_directories>\n");
        exit(EXIT_FAILURE);
    }

    strcpy(path, argv[2]);
    readFile(argv[0], atoi(argv[1]), path);
    kill(getppid(), SIGUSR1);

    return EXIT_SUCCESS;
}
