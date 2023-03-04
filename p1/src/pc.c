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

#define MAX_SIZE 4096

int main(int argc, char const *argv[])
{
    FILE *fp;
    FILE *f_mark;
    char path[MAX_SIZE];
    char *token;
    
    int mark;
    
    float class_media = 0;
    float child_counter = 0;

    // parse_args(argc);
    strcpy(path, argv[2]);

    // readFile(argv[0], atoi(argv[1]), path, 2, argv[3]);

    char line[MAX_SIZE];
    fp  = open_single_file(argv[0],1);

    while (fgets(line, sizeof(line), fp))
    {
        int needed_mark;

        char msg[] = "Mark you should obtain in this new exam to pass is:";
        char first_path[PATH_SIZE];
        token = strtok(line, " ");
        strtok(NULL, " ");
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
        // printf("%s %d\n", msg, needed_mark);
    }

    class_media = class_media / child_counter;
    char class_media_char[10];
    gcvt(class_media, sizeof(class_media_char), class_media_char);
    write(atoi(argv[3]), class_media_char, sizeof(class_media_char));

    fclose(fp);
    return 0;
}
