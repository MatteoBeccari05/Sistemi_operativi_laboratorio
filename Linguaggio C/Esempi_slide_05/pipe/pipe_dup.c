#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
    int pid, fd[2]; 
    char msg[4] = "bye";
    char buffer[4] = {0}; // Buffer per contenere i dati letti
    
    // Creazione della pipe (fd[0] = lettura, fd[1] = scrittura)
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
    
    if (!pid) 
    { 
        /* --- PROCESSO FIGLIO --- */
        close(fd[0]);               // Chiude l'estremità di lettura (non usata)
        close(1);                   // Chiude lo stdout standard
        dup(fd[1]);                 // Ridirige lo stdout sulla pipe (prende il descrittore 1)
        close(fd[1]);               // Chiude il file descriptor originale della pipe
        
        // Scrive sulla pipe usando il descrittore 1 (ora collegato alla pipe)
        write(1, msg, 3); 
    } 
    else 
    { 
        /* --- PROCESSO PADRE --- */
        close(fd[1]);               // Chiude l'estremità di scrittura (non usata)
        
        // Legge dalla pipe e salva nel buffer
        read(fd[0], buffer, 3);        
        close(fd[0]);
        
        // Stampa a schermo il messaggio ricevuto dal figlio
        printf("Il padre ha ricevuto dal figlio tramite la pipe: '%s'\n", buffer);
    }

    return 0;
}