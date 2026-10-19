#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/* 
 * figlio scrive su un file;
 * padre torna all'inizio e legge 
 */

int procfile (char* f1) 
{
    /* f1: file di comunicazione */
    int nread, nwrite = 0, atteso, status, fd, pid;
    char st1[80], st2[80];
    
    if ((fd = open (f1, O_RDWR | O_CREAT, 0644)) < 0) 
    {
        perror("open"); 
        exit(1); 
    }

    if ((pid = fork()) < 0) 
    {
        perror("fork"); 
        exit(2);
    }
    
    if (pid == 0) 
    { /* FIGLIO */
        scanf ("%s", st1);
        nwrite = write (fd, st1, strlen(st1));
        exit (0); 
    }
    else 
    { /* PADRE */
        atteso = wait(&status); /* attesa del figlio */
        lseek (fd, 0L, 0); /* più avanti nel corso ne capiremo la necessità */
        nread = read (fd, st2, 80);
        printf ("Il padre ha letto la stringa %s\n", st2);
        close (fd);
        return (0); 
    }
}

int main (int argc, char* argv[]) 
{
    int integi;
    char file1[] = "file1"; /* dichiarazione/inizializzazione nome file "file1" */
    integi = procfile(file1);
    exit (integi);
}