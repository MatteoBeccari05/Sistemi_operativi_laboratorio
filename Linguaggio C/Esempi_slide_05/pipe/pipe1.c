#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
    int pid;
    char msg[] = "ciao bro";
    int fd[2];
    
    // Creazione della pipe
    if (pipe(fd) == -1) 
    {
        perror("Errore nella creazione della pipe");
        exit(1);
    }
    
    pid = fork();
    if (pid < 0) 
    {
        perror("Errore nella fork");
        exit(1);
    }
    
    if (pid == 0) 
    {
        /* --- PROCESSO FIGLIO (Mittente) --- */
        close(fd[0]);               // Chiude il lato di lettura (non utilizzato dal figlio)
        write(fd[1], msg, 10);      // Scrive i 10 caratteri di "ciao babbo" sulla pipe
        close(fd[1]);               // Chiude il lato di scrittura dopo aver finito
    } 
    else 
    {
        /* --- PROCESSO PADRE (Destinatario) --- */
        close(fd[1]);               // Chiude il lato di scrittura (non utilizzato dal padre)
        
        char buffer[11] = {0};      // Buffer di supporto con spazio per il terminatore '\0'
        read(fd[0], buffer, 10);    // Legge i dati dalla pipe e li salva nel buffer
        close(fd[0]);               // Chiude il lato di lettura dopo aver finito
        
        // Stampa a schermo il messaggio ricevuto dal figlio
        printf("Il padre ha ricevuto: '%s'\n", buffer);
    }

    return 0;
}