#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

// Variabile globale: ogni processo figlio avrà la propria copia indipendente
volatile sig_atomic_t counter = 0;

// Flag globale per interrompere il ciclo del padre alla ricezione di SIGINT
volatile sig_atomic_t stop = 0;

// Gestore SIGUSR1 (figlio): incrementa counter di 1
void handler_usr1(int signum) 
{
    counter++;
}

// Gestore SIGUSR2 (figlio): decrementa counter di 1
void handler_usr2(int signum) 
{
    counter--;
}

// Gestore SIGINT (figlio): stampa il valore di counter e termina
void handler_int_figlio(int signum) 
{
    printf("\nFiglio (PID %d): counter finale = %d\n", getpid(), counter);
    exit(0);
}

// Gestore SIGINT (padre): segnala al ciclo principale di fermarsi
void handler_int_padre(int signum) 
{
    stop = 1;
}

int main(int argc, char **argv)
{
    int Nf;
    int *pids;

    // 1. Controllo argomenti
    if (argc != 2) 
    {
        fprintf(stderr, "Uso: %s <Nf>\n", argv[0]);
        exit(1);
    }

    Nf = atoi(argv[1]);
    if (Nf <= 0) 
    {
        fprintf(stderr, "Errore: Nf deve essere un intero positivo.\n");
        exit(1);
    }

    // Allocazione array per memorizzare i PID degli Nf figli
    pids = malloc(Nf * sizeof(int));
    if (pids == NULL) 
    {
        perror("malloc");
        exit(1);
    }

    // 2. Generazione degli Nf processi figli
    for (int i = 0; i < Nf; i++) 
    {
        int pid = fork();
        if (pid < 0) 
        {
            perror("fork");
            exit(1);
        }

        if (pid == 0) 
        {
            // --- CODICE DEL FIGLIO ---
            free(pids); // Il figlio non usa l'array dei PID

            // Installazione gestori per SIGUSR1, SIGUSR2 e SIGINT con signal()
            if (signal(SIGUSR1, handler_usr1) == SIG_ERR) 
            {
                perror("signal SIGUSR1");
                exit(1);
            }

            if (signal(SIGUSR2, handler_usr2) == SIG_ERR) 
            {
                perror("signal SIGUSR2");
                exit(1);
            }

            if (signal(SIGINT, handler_int_figlio) == SIG_ERR) 
            {
                perror("signal SIGINT figlio");
                exit(1);
            }

            // Attesa passiva dei segnali
            while (1) 
            {
                pause();
            }
        } 
        else 
        {
            // Il padre salva il PID del figlio appena creato
            pids[i] = pid;
        }
    }

    // --- CODICE DEL PADRE ---

    // Installazione gestore SIGINT per il padre con signal()
    if (signal(SIGINT, handler_int_padre) == SIG_ERR) 
    {
        perror("signal SIGINT padre");
        exit(1);
    }

    // Inizializzazione generatore numeri casuali
    srand(time(NULL));

    // Ogni 2 secondi seleziona casualmente un figlio e un segnale tra SIGUSR1 e SIGUSR2
    while (!stop) 
    {
        sleep(2);
        if (stop) 
        {
            break;
        }

        int idx = rand() % Nf;
        int sig = (rand() % 2 == 0) ? SIGUSR1 : SIGUSR2;

        printf("Padre: invio %s al figlio %d (PID %d)\n",
               (sig == SIGUSR1) ? "SIGUSR1" : "SIGUSR2", idx, pids[idx]);

        if (kill(pids[idx], sig) < 0) 
        {
            perror("kill");
        }
    }

    // Attesa della terminazione di tutti i figli prima di chiudere il padre
    for (int i = 0; i < Nf; i++) 
    {
        wait(NULL);
    }

    free(pids);
    printf("Padre: tutti gli %d figli sono terminati. Esco.\n", Nf);
    return 0;
}