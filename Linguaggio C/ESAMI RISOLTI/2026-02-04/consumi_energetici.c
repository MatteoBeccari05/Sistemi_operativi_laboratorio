#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#define DIM_GIORNO 5       // per la stringa "MMDD" + terminatore
#define DIM_EDIFICIO 30    // edificio senza spazi
#define DIM_PATH 256

static volatile sig_atomic_t count = 0;

void handler_sigint()
{
    printf("numero di richieste: %d\n", count);
    exit(0);
}

void handler_sigusr1() {
    // Handler vuoto, serve solo a sbloccare la pause() in P1
}

int main(int argc, char *argv[])
{
    int p1p2[2], p2p3[2], pid1, pid2, pid3, status;
    char giorno[DIM_GIORNO], edificio[DIM_EDIFICIO], path[DIM_PATH];

    // controllo numero argomenti (2): ./consumi_energetici <anno>
    if (argc != 2)
    {
        printf("errore: numero sbagliato di argomenti\n");
        exit(1);
    }

    // controllo che argv[1] sia un path relativo
    if (argv[1][0] == '/')
    {
        printf("errore: path assoluto\n");
        exit(2);
    }

    // controllo che argv[1] sia un path esistente
    if (opendir(argv[1]) == NULL)
    {
        printf("errore: path non esistente\n");
        exit(3);
    }

    // imposto gestore SIGINT (ctrl-C)
    signal(SIGINT, handler_sigint);

    // ciclo infinito delle richieste
    while (1)
    {
        printf("inserire il giorno (MMDD): ");
        scanf("%s", giorno);

        // controllo che <dir>/<giorno>.eng esista
        sprintf(path, "%s/%s.eng", argv[1], giorno);
        if (open(path, O_RDONLY) < 0)
        {
            printf("errore: file %s non esistente\n", path);
            continue;
        }

        printf("inserire l'edificio: ");
        scanf("%s", edificio);

        // creo la pipe p1->p2
        if (pipe(p1p2) < 0)
        {
            printf("errore: creazione pipe p1p2\n");
            exit(4);
        }

        // creo processo figlio p1
        pid1 = fork();
        if (pid1 < 0)
        {
            printf("errore: creazione figlio p1\n");
            exit(5);
        }
        if (pid1 == 0)
        {
            // p1: gestione segnale
            signal(SIGINT, SIG_DFL);
            signal(SIGUSR1, handler_sigusr1);

            // mi sospendo in attesa di SIGUSR1
            pause();

            // chiudo estremità non usate della pipe p1p2
            close(p1p2[0]);

            // redirigo stdout su p1p2 (scrittura)
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);

            // eseguo grep sull'edificio e sul file .eng
            execlp("grep", "grep", edificio, path, (char*)0);

            // se arrivo qui, errore
            printf("errore: grep (p1)\n");
            exit(6);
        }

        // creo la pipe p2->p3
        if (pipe(p2p3) < 0)
        {
            printf("errore: creazione pipe p2p3\n");
            exit(7);
        }

        // creo processo figlio p2
        pid2 = fork();
        if (pid2 < 0)
        {
            printf("errore: creazione figlio p2\n");
            exit(8);
        }
        if (pid2 == 0)
        {
            // p2: gestione segnale
            signal(SIGINT, SIG_DFL);

            // chiudo estremità inutilizzate
            close(p1p2[1]);
            close(p2p3[0]);

            // redirigo stdin = p1p2[0]
            close(0);
            dup(p1p2[0]);
            close(p1p2[0]);

            // redirigo stdout = p2p3[1]
            close(1);
            dup(p2p3[1]);
            close(p2p3[1]);

            // ordino le righe in ordine decrescente sul primo campo numerico
            execlp("sort", "sort", "-t", ",", "-n", "-r", (char*)0);

            printf("errore: sort (p2)\n");
            exit(9);
        }

        // chiudo p1p2 subito dopo aver forkato p2
        close(p1p2[0]);
        close(p1p2[1]);

        // creo processo figlio p3
        pid3 = fork();
        if (pid3 < 0)
        {
            printf("errore: creazione figlio p3\n");
            exit(10);
        }
        if (pid3 == 0)
        {
            // p3: gestione segnale
            signal(SIGINT, SIG_DFL);

            // chiudo estremità inutilizzate
            close(p2p3[1]);

            // redirigo stdin = p2p3[0]
            close(0);
            dup(p2p3[0]);
            close(p2p3[0]);

            // p3 deve stampare solo quantità e orario (campi 1 e 2)
            execlp("cut", "cut", "-d,", "-f1,2", (char*)0);

            printf("errore: cut (p3)\n");
            exit(11);
        }

        // chiudo p2p3 nel processo padre
        close(p2p3[0]);
        close(p2p3[1]);

        // mando il segnale SIGUSR1 al figlio p1
        sleep(1);
        kill(pid1, SIGUSR1);

        // attendo la terminazione dei 3 figli
        wait(&status);
        wait(&status);
        wait(&status);

        count++;
    }

    return 0; // non raggiunto, il programma termina con ctrl-C
}