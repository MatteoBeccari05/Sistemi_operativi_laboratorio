#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 100

int main() 
{
    int pid[N], status, i, k;
    for (i = 0; i < N; i++)
    {
        pid[i] = fork();
        if (pid[i] == 0)
        {
            printf("figlio: il mio pid è: %d\n", getpid());
            /* ... */
            exit(0);
        }
    }

    /* continua (codice padre).. */

    for (i = 0; i < N; i++)   /* attesa di tutti i figli */
    { 
        k = wait(&status);
        if (WIFEXITED(status))
            printf("Term. volontaria di %d con stato %d\n", k, WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("term. Involontaria di %d per segnale %d\n", k, WTERMSIG(status));
    }
    
    return 0;
}