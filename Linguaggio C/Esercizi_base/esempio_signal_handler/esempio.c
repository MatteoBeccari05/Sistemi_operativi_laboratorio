#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// Handler per la gestione del figlio terminato
void handler(int signum) 
{
    int status;
    wait(&status); // Raccoglie lo stato di terminazione senza bloccare il padre a priori
    
    // Sposta i bit per leggere il vero valore di exit() del figlio
    printf("Stato terminazione figlio: %d\n", status >> 8);
}

int main() 
{
    int PID, i;
    
    PID = fork();
    
    if (PID > 0) 
    { 
        /* --- PROCESSO PADRE --- */
        // Associa il segnale SIGCHLD all'handler
        signal(SIGCHLD, handler);
        
        // Il padre esegue un ciclo molto lungo senza mai chiamare wait() nel main()
        printf("Padre (PID %d) in esecuzione...\n", getpid());
        for (i = 0; i < 1000000000; i++); /* Lunga attività del padre */
        
        printf("Padre terminato.\n");
        exit(0);
    } 
    else 
    { 
        /* --- PROCESSO FIGLIO --- */
        // Il figlio esegue un'attività breve e poi termina
        printf("Figlio (PID %d) in esecuzione...\n", getpid());
        for (i = 0; i < 1000000; i++);    /* Breve attività del figlio */
        
        printf("Figlio terminato. Mando SIGCHLD al padre.\n");
        exit(1); // Termina restituendo lo stato 1
    }
}