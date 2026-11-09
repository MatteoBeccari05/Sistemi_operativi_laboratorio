#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Uso: %s <nomefile> <stringa1> ... <stringaN>\n", argv[0]);
        return 1;
    }

    char *nomefile = argv[1];

    int fd = open("conteggi.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) 
    {
        perror("Errore creazione file di log");
        return 1;
    }
    close(fd);

    // 3. Creo un processo figlio per ogni stringa (in modo CONCORRENTE)
    for (int i = 2; i < argc; i++) 
    {
        int pid = fork();

        if (pid < 0) 
        {
            perror("Errore fork");
            return 1;
        }

        if (pid == 0) 
        { // CODICE DEL FIGLIO
            // a. Apro conteggi.txt in modalità APPEND
            int fd_log = open("conteggi.txt", O_WRONLY | O_APPEND);
            if (fd_log >= 0) 
            {
                // Scrivo la riga: "<nomefile> <stringa>\n"
                char log_str[512];
                snprintf(log_str, sizeof(log_str), "%s %s\n", nomefile, argv[i]);
                write(fd_log, log_str, strlen(log_str));
                close(fd_log);
            }

            // b. Eseguo il conteggio usando l'utility di sistema 'grep -c'
            printf("Occorrenze di '%s': ", argv[i]);
            fflush(stdout); // Svuota il buffer prima della exec

            // execlp esegue: grep -c <stringa> <nomefile>
            execlp("grep", "grep", "-c", argv[i], nomefile, NULL);

            // Se execlp fallisce
            perror("Errore execlp");
            exit(1);
        }
    }

    // 4. CODICE DEL PADRE: attende la fine di tutti i figli generati
    for (int i = 2; i < argc; i++) 
    {
        wait(NULL);
    }

    return 0;

}