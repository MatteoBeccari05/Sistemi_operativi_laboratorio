#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Variabile globale per permettere al gestore del segnale di leggerla
volatile sig_atomic_t counter = 0;

void sighandlerUSR1(int signum) 
{
    printf("Figlio (PID %d): iterazioni compiute = %d\n", getpid(), counter);
    exit(0);
}

int main(int argc, char *argv[]) 
{
    int Nsec;
    int pid;

    // 1. Controllo argomenti
    if (argc != 2) 
    {
        fprintf(stderr, "Uso: %s <Nsec>\n", argv[0]);
        exit(1);
    }

    //controllo numero positivo
    Nsec = atoi(argv[1]);
    if (Nsec <= 0) 
    {
        fprintf(stderr, "Errore: Nsec deve essere un numero intero positivo.\n");
        exit(1);
    }

    //generazione processo figlio
    pid = fork();
    if(pid < 0)
    {
        perror("fork");
        exit(1);
    }

    //codice figlio
    if(pid == 0)
    {
        // Installazione del gestore per SIGUSR1 con sigaction
        struct sigaction sa;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sa.sa_handler = sighandlerUSR1;

        if(sigaction(SIGUSR1, &sa, NULL) < 0)
        {
            perror("sigaction");
            exit(1);
        }

        // Ciclo infinito: dorme 1 secondo e incrementa il contatore
        while(1)
        {
            sleep(1);
            counter++;
        }
    }
    else  //CODICE PADRE
    {
        // Il padre dorme per Nsec secondi
        sleep(Nsec);

        // Invio di SIGUSR1 allo specifico processo figlio
        if (kill(pid, SIGUSR1) < 0) 
        {
            perror("kill");
            exit(1);
        }

        // Attesa della terminazione del figlio
        wait(NULL);
        printf("Padre: figlio terminato correttamente.\n");
    }   
    return 0;
}