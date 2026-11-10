#include <stdio.h>
#include <stdlib.h>     
#include <unistd.h>     
#include <sys/wait.h>  

int main(void) 
{
    int pid;
    int status;
    int pid_terminato;

    printf("[PADRE] Inizio esecuzione. Il mio PID e': %d\n", getpid());

    /* 1. Creazione del processo figlio */
    pid = fork();

    /* Caso A: Errore nella fork() */
    if (pid < 0) 
    {
        perror("Errore durante la fork");
        exit(1);
    }

    /* Caso B: Codice eseguito dal PROCESSO FIGLIO */
    if (pid == 0) 
    {
        printf("[FIGLIO] Sono stato creato! Mio PID: %d, PID di mio padre: %d\n", getpid(), getppid());
        printf("[FIGLIO] Ora sostituisco il mio codice con '/bin/ls -l' usando execl()...\n\n");

        /* 2. Sostituzione del codice con execl() */
        execl("/bin/ls", "ls", "-l", (char *)0);

        /* 
         * NOTA: Se execl() ha successo, il codice sottostante NON verra' mai eseguito
         * perche' l'immagine del processo figlio viene sovrascritta da "/bin/ls".
         * Se arriviamo qui, significa che execl() ha fallito!
         */
        perror("[FIGLIO] Errore nell'esecuzione di execl");
        exit(2); /* Termina il figlio ritornando il codice di errore 2 al padre */
    }

    /* Caso C: Codice eseguito dal PROCESSO PADRE (pid > 0) */
    if (pid > 0) 
    {
        printf("[PADRE] Ho generato il figlio con PID %d. Mi sospendo con wait()...\n", pid);

        /* 3. Attesa della terminazione del figlio */
        pid_terminato = wait(&status);

        if (pid_terminato < 0) 
        {
            perror("[PADRE] Errore nella wait");
            exit(1);
        }

        printf("\n[PADRE] Il processo figlio con PID %d e' terminato.\n", pid_terminato);

        /* 4a. Lettura dello status tramite Macro POSIX.1 */
        if (WIFEXITED(status)) 
        {
            printf("[PADRE - Macro POSIX] Terminazione volontaria (exit) con stato: %d\n", WEXITSTATUS(status));
        } 
        else if (WIFSIGNALED(status)) 
        {
            printf("[PADRE - Macro POSIX] Terminazione involontaria da segnale numero: %d\n", WTERMSIG(status));
        }

        /* 4b. Lettura dello status tramite manipolazione diretta dei bit (16 bit) */
        if ((char)status == 0) 
        {
            /* Il byte meno significativo e' 0 -> terminazione volontaria */
            int exit_code = status >> 8;
            printf("[PADRE - Bitwise]     Terminazione volontaria con stato: %d\n", exit_code);
        } 
        else 
        {
            /* Il byte meno significativo contiene il numero del segnale */
            int sig_num = (char)status;
            printf("[PADRE - Bitwise]     Terminazione involontaria per segnale: %d\n", sig_num);
        }

        printf("[PADRE] Termino anch'io con successo.\n");
        exit(0);
    }

    return 0;
}


/*
In questo programma, il processo padre genera un figlio a cui delega l'esecuzione del comando UNIX /bin/ls -l. 
Il padre si sospende con wait() fino alla terminazione del figlio e poi ne analizza lo stato di uscita.
*/