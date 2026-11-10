#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int ntimes = 0;

void handler(int signo) 
{
    ntimes++; // Contatore (Nota: non è atomico/volatile in questo esempio base)
    printf("Processo %d ricevuto # %d volte il segnale %d\n", getpid(), ntimes, signo); 
}

int main() {
    int pid, ppid;
    
    signal(SIGUSR1, handler); 
    
    if ((pid = fork()) < 0) 
    {
        exit(1); /* fork fallita */ 
    } 
    else if (pid == 0) 
    { /* figlio */ 
        ppid = getppid(); /* PID del padre */ 
        for (;;) 
        {
            printf("FIGLIO %d\n", getpid()); 
            sleep(1);                        
            kill(ppid, SIGUSR1);            
            pause();                         
        }
    } 
    else 
    { /* padre */
        for (;;) 
        { /* ciclo infinito */      
            printf("PADRE %d\n", getpid());  
            pause();                         
            sleep(1);                       
            kill(pid, SIGUSR1);              
        }
    }
}