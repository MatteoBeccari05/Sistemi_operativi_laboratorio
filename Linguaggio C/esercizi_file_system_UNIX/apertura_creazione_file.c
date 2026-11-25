#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) 
{
    int fd1, fd2, fd3;

    fd1 = open("ff.txt", O_RDONLY);
    if (fd1 < 0) 
    {
        perror("open ff.txt fallita");
    } 
    else 
    {
        close(fd1);
    }

    fd2 = open("f2.new", O_WRONLY);
    if (fd2 < 0) 
    {
        perror("open in scrittura fallita, creo il file");
        fd2 = open("f2.new", O_WRONLY | O_CREAT, 0777);
        // Equivalente a: fd2 = creat("f2.new", 0777);
    }
    if (fd2 >= 0) 
    {
        close(fd2);
    }

    /* Omogeneità apertura dispositivo di output */
    fd3 = open("/dev/null", O_WRONLY);
    if (fd3 < 0) 
    {
        perror("open dispositivo fallita");
    } 
    else 
    {
        close(fd3);
    }

    return 0;
}