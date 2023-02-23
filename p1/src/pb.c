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

    // parse_args(argc);
    strcpy(path, argv[3]);
   
    readFile(argv[1], atoi(argv[2]), path,1);
    printf("[PB] Finished reading file.\n");
    // kill(getppid(), SIGUSR2);
    return 0;
}
