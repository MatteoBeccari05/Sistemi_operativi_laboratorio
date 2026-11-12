#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Contatore globale modificato dall'handler (volatile sig_atomic_t evita ottimizzazioni errate)
volatile sig_atomic_t tentativi_uscita = 0;

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
    if (signal(SIGINT, gestore_sigint) == SIG_ERR) 
    {
        perror("sigaction");
        exit(1);
    }

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