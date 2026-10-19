#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char *env[]={"USER=matteo", "PATH=/home/matteo/", (char *)0};
char *argv[]={"ls", "-l", (char *)0};

int main()
{
    int pid, status;
    pid=fork();
    if (pid==0)
    {
        execve("/bin/ls", argv, env);
        printf("exec fallita!\n");
        exit(1);
    }
    else if (pid >0)
    {
        pid=wait(&status); /* gestione dello stato.. */
    }
    else printf("fork fallita!\n");
}