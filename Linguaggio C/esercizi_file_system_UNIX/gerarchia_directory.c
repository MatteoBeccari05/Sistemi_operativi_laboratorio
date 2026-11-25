#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void esplora(char *percorso, char *f) 
{
    char nd[1024];
    DIR *dir;
    struct dirent *ff;

    /* Apre "." perché chdir() ha già spostato il processo nella directory corrente */
    dir = opendir(".");
    if (dir == NULL) 
    {
        perror("Errore opendir");
        return;
    }

    while ((ff = readdir(dir)) != NULL) 
    {
        if ((strcmp(ff->d_name, ".") != 0) && (strcmp(ff->d_name, "..") != 0)) 
        {
            if (chdir(ff->d_name) != 0) 
            {
                /* È un file (o directory non attraversabile): confronta il nome */
                if (strcmp(f, ff->d_name) == 0) 
                {
                    printf("File %s nel dir %s\n", f, percorso);
                }
            } 
            else 
            {
                /* È una directory e chdir() ci è già entrato */
                snprintf(nd, sizeof(nd), "%s/%s", percorso, ff->d_name);
                esplora(nd, f);
                chdir(".."); /* Risale al livello padre al ritorno dalla chiamata ricorsiva */
            }
        }
    }

    closedir(dir);
}

int main(int argc, char **argv) 
{
    if (argc != 3) 
    {
        printf("Errore parametri. Uso: %s <radice> <file>\n", argv[0]);
        exit(1);
    }

    if (chdir(argv[1]) != 0) 
    {
        perror("Errore in chdir");
        exit(1);
    }

    esplora(argv[1], argv[2]);
    return 0;
}