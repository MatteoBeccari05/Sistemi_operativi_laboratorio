#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
    int pid, status;
    pid = fork();

    if (pid == 0) 
    {
        printf("figlio\n");
        exit(0);
    }
    else 
    {
        pid = wait(&status);
        printf("terminato processo figlio n.%d\n", pid);
        if ((char)status == 0)
            printf("term. volontaria con stato %d\n", status >> 8);
        else 
            printf("terminazione involontaria per segnale %d\n", (char)status);
    }

    return 0;
}