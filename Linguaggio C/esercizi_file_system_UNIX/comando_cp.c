#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFDIM 1000
#define PERM 0777

int main(int argc, char **argv) 
{
    int infile, outfile, nread;
    char buffer[BUFDIM];

    if (argc != 3) 
    {
        printf("Uso: %s <destinazione> <sorgente>\n", argv[0]);
        exit(1);
    }

    if ((infile = open(argv[2], O_RDONLY)) < 0) 
    {
        perror("Apertura sorgente");
        exit(1);
    }

    if ((outfile = creat(argv[1], PERM)) < 0) 
    {
        perror("Apertura destinazione");
        close(infile);
        exit(1);
    }

    while ((nread = read(infile, buffer, BUFDIM)) > 0) 
    {
        if (write(outfile, buffer, nread) < nread) 
        {
            perror("Errore in scrittura");
            close(infile);
            close(outfile);
            exit(1);
        }
    }

    close(infile);
    close(outfile);
    return 0;
}