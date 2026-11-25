#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void miols(char name[]) 
{
    DIR *dir;
    struct dirent *dd;
    char buff[300];

    dir = opendir(name);
    if (dir == NULL) 
    {
        perror("Errore opendir");
        return;
    }

    while ((dd = readdir(dir)) != NULL) 
    {
        snprintf(buff, sizeof(buff), "%s\n", dd->d_name);
        write(1, buff, strlen(buff));
    }

    closedir(dir);
}

int main(int argc, char **argv) 
{
    if (argc <= 1) 
    {
        printf("Errore: specificare una directory\n");
        exit(1);
    }

    miols(argv[1]);
    return 0;
}