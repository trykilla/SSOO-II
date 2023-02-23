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
#include "../include/my_lib.h"

#define MAX_SIZE 4096



int main(int argc, char const *argv[])
{
    char path[MAX_SIZE];
    parse_args(argc);
    strcpy(path, argv[2]);

    readFile(argv[0], atoi(argv[1]), path, 2);

    return 0;
}
