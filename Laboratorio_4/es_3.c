#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{
    // 1. Controllo argomenti: deve essere specificato <num>
    if (argc != 2) 
    {
        printf("Uso: %s <num>\n", argv[0]);
        return 1;
    }

    char *num_righe = argv[1];
    char nomefile[256];

    while (1) 
    {
        // 2. Lettura del nome del file da tastiera
        printf("Inserisci il nome del file (o 'fine' per uscire): ");
        if (fgets(nomefile, sizeof(nomefile), stdin) == NULL) break;

        // Rimuovo l'a capo '\n' finale
        nomefile[strcspn(nomefile, "\n")] = '\0';

        // Condizione di uscita
        if (strcmp(nomefile, "fine") == 0) 
        {
            break;
        }

        // 3. Verifica esistenza del file
        if (access(nomefile, F_OK) != 0) 
        {
            printf("Errore: il file '%s' non esiste.\n\n", nomefile);
            continue;
        }

        // 4. Creazione processo figlio per la visualizzazione
        pid_t pid = fork();

        if (pid < 0) 
        {
            perror("Errore fork");
            return 1;
        }

        if (pid == 0) 
        { // CODICE DEL FIGLIO
            // Esegue il comando di sistema: head -n <num> <nomefile>
            execlp("head", "head", "-n", num_righe, nomefile, NULL);

            // Se execlp fallisce
            perror("Errore execlp");
            exit(1);
        } 
        else 
        { // CODICE DEL PADRE
            // Attende la fine della stampa da parte del figlio prima di proseguire
            wait(NULL);
            printf("\n");
        }
    }
    return 0;
}