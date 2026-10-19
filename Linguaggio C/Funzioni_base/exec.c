#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
    int pid;

    pid = fork();

    /* 1. Controllo immediato se la fork è fallita */
    if (pid < 0) 
    { /* fork fallita */
        perror("Errore in fork\n");
        exit(1);
    }
    
    /* 2. Codice eseguito dal figlio */
    if (pid == 0) 
    { /* figlio */
        printf("Figlio: esecuzione di ls\n");
        execl("/bin/ls", "ls", "-l", (char *)0);
        perror("Errore in execv\n"); 
        exit(1);
    }
    
    /* 3. Codice eseguito dal padre */
    if (pid > 0) 
    { /* padre */
        // Eventuale wait(&status) consigliata qui per evitare processi zombie
        printf("Padre ....\n");
        exit(0);
    }

    return 0;
}