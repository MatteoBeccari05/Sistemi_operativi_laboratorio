#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) 
{
    if (argc != 3) 
    {
        printf("Sintassi errata. Uso: %s <vecchio_nome> <nuovo_nome>\n", argv[0]);
        exit(1);
    }

    if (link(argv[1], argv[2]) < 0) 
    {
        perror("Errore link");
        exit(1);
    }

    if (unlink(argv[1]) < 0) 
    {
        perror("Errore unlink");
        exit(1);
    }

    return 0;
}