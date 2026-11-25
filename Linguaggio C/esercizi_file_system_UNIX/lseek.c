#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) 
{
    int fd, n;
    char buf[100];
    const char *filename = (argc > 1) ? argv[1] : "miofile.txt";

    if ((fd = open(filename, O_RDWR)) < 0) 
    {
        perror("Errore di apertura");
        exit(1);
    }

    /* Posizionamento sul terzultimo byte del file (2 == SEEK_END) */
    if (lseek(fd, -3, SEEK_END) < 0) 
    {
        perror("Errore in lseek");
        close(fd);
        exit(1);
    }

    n = read(fd, buf, 3);
    if (n > 0) 
    {
        write(1, buf, n);
        write(1, "\n", 1);
    }

    close(fd);
    return 0;
}