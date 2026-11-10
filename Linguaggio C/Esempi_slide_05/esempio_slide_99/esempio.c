/* sintassi: programma com1 com2 (significa: com1 | com2) */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) 
{
    int pid1, pid2, fd[2], i, status;
    
    // Controllo argomenti di input
    if (argc != 3) 
    {
        fprintf(stderr, "Uso corretto: %s <com1> <com2>\nEsempio: %s ls wc\n", argv[0], argv[0]);
        exit(1);
    }

    printf("[PADRE] Avvio. Creo la pipe per collegare '%s' e '%s'...\n", argv[1], argv[2]);
    pipe(fd); // Crea la pipe (fd[0] lettura, fd[1] scrittura)
    
    pid1 = fork();
    if (!pid1) { 
        /* --- PRIMO PROCESSO FIGLIO: esegue com2 (Destinatario) --- */
        fprintf(stderr, "[FIGLIO 1 - Destinatario] Avviato per eseguire '%s'.\n", argv[2]);
        
        close(fd[1]);               // Chiude il lato di scrittura (non serve)
        close(0);                   // Chiude lo stdin originale (fd 0)
        
        dup(fd[0]);                 /* Ridirigo stdin sulla pipe: fd 0 ora legge da fd[0] */
        close(fd[0]);               // Chiude il fd[0] originale
        
        fprintf(stderr, "[FIGLIO 1 - Destinatario] Stdin collegato alla pipe. Eseguo '%s'...\n", argv[2]);
        
        // Sostituisce il codice del processo con il comando com2
        execlp(argv[2], argv[2], (char *)0);
        perror("Errore execlp com2");
        exit(-1);
    } 
    else 
    { 
        /* --- PROCESSO PADRE --- */
        pid2 = fork();
        
        if (!pid2) 
        { 
            /* --- SECONDO PROCESSO FIGLIO: esegue com1 (Mittente) --- */
            fprintf(stderr, "[FIGLIO 2 - Mittente] Avviato per eseguire '%s'.\n", argv[1]);
            
            close(fd[0]);           // Chiude il lato di lettura (non serve)
            close(1);               // Chiude lo stdout originale (fd 1)
            
            dup(fd[1]);             /* Ridirigo stdout sulla pipe: fd 1 ora scrive su fd[1] */
            close(fd[1]);           // Chiude il fd[1] originale
            
            fprintf(stderr, "[FIGLIO 2 - Mittente] Stdout collegato alla pipe. Eseguo '%s'...\n", argv[1]);
            
            // Sostituisce il codice del processo con il comando com1
            execlp(argv[1], argv[1], (char *)0);
            perror("Errore execlp com1");
            exit(-1);
        }
        
        /* IL PROCESSO PADRE CHIUDE LA PIPE */
        printf("[PADRE] Entrambi i figli creati. Chiudo i descrittori di pipe non usati nel padre.\n");
        close(fd[0]);
        close(fd[1]);
        
        /* Il padre attende la terminazione di entrambi i figli */
        printf("[PADRE] In attesa che i figli completino l'esecuzione...\n");
        for (i = 0; i < 2; i++) 
        {
            int pid_finito = wait(&status);
            if ((char)status != 0) 
            {
                printf("[PADRE] Figlio (PID %d) terminato per segnale %d\n", pid_finito, (char)status);
            } 
            else 
            {
                printf("[PADRE] Figlio (PID %d) terminato con successo (stato %d)\n", pid_finito, status >> 8);
            }
        }
        printf("[PADRE] Esecuzione completata.\n");
        exit(0);
    } 
} 