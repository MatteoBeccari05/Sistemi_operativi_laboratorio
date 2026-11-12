#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Contatore globale modificato dall'handler (volatile sig_atomic_t evita ottimizzazioni errate)
volatile sig_atomic_t tentativi_uscita = 0;

// 1. DEFINIZIONE DEL SIGNAL HANDLER
// Questa funzione NON viene mai chiamata direttamente nel main:
// viene invocata automaticamente dal sistema operativo quando arriva il segnale.
void gestore_sigint(int signum) 
{
    tentativi_uscita++;

    printf("\n[HANDLER] Segnale %d (SIGINT) intercettato! Tentativo %d di 3.\n", signum, tentativi_uscita);

    if (tentativi_uscita >= 3) 
    {
        printf("[HANDLER] Raggiunti 3 tentativi. Termino il programma.\n");
        exit(0);
    }

    printf("[HANDLER] Ignoro l'uscita e torno al ciclo principale...\n");
}

int main() 
{
    // 2. INSTALLAZIONE DEL GESTORE CON sigaction()
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);          // Nessun segnale extra bloccato durante l'handler
    sa.sa_flags = 0;                   // Comportamento standard
    sa.sa_handler = gestore_sigint;    // Funzione da eseguire all'arrivo di SIGINT

    if (sigaction(SIGINT, &sa, NULL) < 0) 
    {
        perror("sigaction");
        exit(1);
    }
    // Nota: in alternativa si potrebbe scrivere signal(SIGINT, gestore_sigint);
    // ma sigaction() è preferibile per maggiore portabilità tra sistemi UNIX/Linux.

    // 3. FLUSSO NORMALE DEL PROGRAMMA
    int secondi = 0;
    printf("Processo avviato (PID %d). Premi CTRL-C per inviare SIGINT.\n", getpid());

    while (1) 
    {
        printf("Esecuzione normale... secondi trascorsi: %d\n", secondi++);
        sleep(1);
    }

    return 0;
}