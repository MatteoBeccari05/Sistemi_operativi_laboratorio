#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int cont = 0;

void handler(int signo) 
{
    // Stampa il PID del processo, il contatore e il numero del segnale
    printf("Proc. %d: ricevuti n. %d segnali %d\n", getpid(), cont, signo);
}

int main() 
{
    int pid;
    
    // Associa la funzione handler al segnale SIGUSR1
    signal(SIGUSR1, handler);
    
    pid = fork();
    
    if (pid == 0) 
    {
        /* PROCESSO FIGLIO */
        // Ciclo infinito in attesa di ricevere segnali
        for (;;);
    } 
    else 
    {
        /* PROCESSO PADRE */
        // Invia ripetutamente il segnale SIGUSR1 al processo figlio
        for (;;) 
        {
            kill(pid, SIGUSR1);
        }
    }
}