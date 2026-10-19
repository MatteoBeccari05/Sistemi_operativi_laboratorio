#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int pid, status;
    pid=fork();
    if (pid==0)
    {
        execl("/home/paolo/prova", "prova",(char *)0);
        perror("exec fallita a causa dell’errore:");
        exit(1);
    }
}