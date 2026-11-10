/* Sintassi: progr N */
/* Padre (destinatario) e figlio (mittente) si scambiano messaggi di dimensione (DIM) costante.
   La lunghezza della sequenza non è nota a priori.
   Il destinatario interrompe lo scambio dopo N secondi. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define DIM 10

int fd[2]; // Pipe dichiarata globale per renderla accessibile agli handler

// Prototipi degli handler
void fine(int signo);
void timeout(int signo);

int main(int argc, char **argv) 
{
    int pid, N;
    char messaggio[] = "ciao ciao ";

    // Controllo argomenti
    if (argc != 2) 
    {
        printf("Errore di sintassi\n");
        exit(1);
    }
    
    N = atoi(argv[1]); // Converte il primo argomento (secondi) in intero
    pipe(fd);          // Crea la pipe
    pid = fork();      // Genera il processo figlio

    if (pid == 0) 
    { 
        /* --- PROCESSO FIGLIO (Mittente) --- */
        signal(SIGPIPE, fine); // Gestisce la "broken pipe"
        close(fd[0]);          // Chiude il lato di lettura non utilizzato

        for(;;) 
        {
            // Scrittura infinita sulla pipe
            write(fd[1], messaggio, DIM);
        }
    } 
    else if (pid > 0) 
    { 
        /* --- PROCESSO PADRE (Destinatario) --- */
        signal(SIGALRM, timeout); // Gestisce il timeout
        close(fd[1]);             // Chiude il lato di scrittura non utilizzato
        
        alarm(N);                 // Imposta il timer di N secondi
        
        for(;;) 
        {
            // Lettura infinita dalla pipe e stampa a video (fd 1)
            read(fd[0], messaggio, DIM);
            write(1, messaggio, DIM);
        }
    }
    
    return 0;
}

/* --- DEFINIZIONE DEGLI HANDLER DEI SEGNALI --- */

// Handler del padre: scatta quando scade l'alarm(N)
void timeout(int signo) 
{
    int stato;
    close(fd[0]); /* Chiusura effettiva del lato di lettura: causerà SIGPIPE nel figlio */
    wait(&stato); /* Attende la terminazione del figlio */
    
    if ((char)stato != 0)
        printf("\nTermin invol figlio (segnale %d)\n", (char)stato);
    else 
        printf("\nTermin volont Figlio (stato %d)\n", stato >> 8);
        
    exit(0);
}

// Handler del figlio: scatta quando cerca di scrivere su una pipe senza lettori
void fine(int signo) 
{
    close(fd[1]); /* Chiude il lato di scrittura */
    exit(0);      /* Termina in modo pulito */
}