#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) 
{
    int fd, n;
    char buf[10];
    const char *filename = (argc > 1) ? argv[1] : "miofile.txt";

    if ((fd = open(filename, O_RDONLY)) < 0) 
    {
        perror("Errore di apertura");
        exit(1);
    }

    while ((n = read(fd, buf, 10)) > 0) 
    {
        write(1, buf, n); // scrittura sullo standard output (fd = 1)
    }

    close(fd);
    return 0;
}