#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int pid;
    pid=fork();
    if (pid==0)
    {
        /* codice figlio */
        printf("Sono il figlio ! (pid: %d)\n", getpid());
    }
    else if (pid>0)
    {
        /* codice padre */
        printf("Sono il padre: pid del figlio: %d\n", pid);
    }
    else printf("Creazione fallita! ");
}