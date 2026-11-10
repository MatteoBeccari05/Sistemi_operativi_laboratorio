#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

//esempio: mediante dup() ridirigere stdout su pipe
int main() 
{
    int pid, fd[2]; 
    char msg[4] = "bye";
    char buffer[10] = {0}; // Buffer in cui il padre riceverà i dati
    
    pipe(fd);
    pid = fork();
    
    if (!pid) 
    { 
        /* --- PROCESSO FIGLIO --- */
        fprintf(stderr, "[FIGLIO] Avviato. Chiudo fd[0] e lo stdout standard (fd 1).\n");
        close(fd[0]);               // Chiude il lato di lettura
        close(1);                   // Chiude lo stdout originale (il video)
        
        dup(fd[1]);                 /* Ridirigo stdout sulla pipe: 1 ora punta a fd[1] */
        close(fd[1]);               // Chiude il fd[1] originale (ormai duplicato in fd 1)
        
        fprintf(stderr, "[FIGLIO] Stdout ridiretto sulla pipe. Invio il messaggio...\n");
        
        /* Scrive "bye" sulla pipe usando il file descriptor 1 (stdout ridiretto) */
        write(1, msg, sizeof(msg)); 
        
        fprintf(stderr, "[FIGLIO] Messaggio scritto sulla pipe! Chiudo il descrittore 1.\n");
        close(1);
    } 
    else 
    { 
        /* --- PROCESSO PADRE --- */
        close(fd[1]);               // Chiude il lato di scrittura
        
        printf("[PADRE] Avviato. In attesa di dati dalla pipe (fd[0])...\n");
        
        // Legge 3 byte dalla pipe
        int n = read(fd[0], buffer, 3); 
        buffer[n] = '\0';           // Inserisce il terminatore di stringa per la stampa
        
        printf("[PADRE] Messaggio letto dalla pipe: \"%s\" (%d byte letti).\n", buffer, n);
        
        close(fd[0]);
        wait(NULL);                 // Attende la terminazione del figlio
        printf("[PADRE] Esecuzione completata.\n");
    } 
    
    return 0;
}