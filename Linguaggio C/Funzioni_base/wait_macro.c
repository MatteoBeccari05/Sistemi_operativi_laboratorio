#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pid, status;
    pid = fork();

    if(pid==0)
    {
        printf("figlio\n");
        exit(0);
    }
    else
    {
        pid=wait(&status);
        if (WIFEXITED(status))
            printf("Terminazione volontaria di %d con stato %d\n", pid, WEXITSTATUS(status));

        else if (WIFSIGNALED(status))
            printf("terminazione involontaria per segnale %d\n", WTERMSIG(status));

    }
    return 0;
}