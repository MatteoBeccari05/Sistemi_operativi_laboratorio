#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#define DIM_GIORNO   5
#define DIM_MATRICOLA 50
#define DIM_PATH     256

static volatile sig_atomic_t count = 0;

void handler_sigint()
{
    printf("numero di richieste: %d\n", count);
    exit(0);
}

int main(int argc, char *argv[])
{
    int p1p2[2], p2p3[2], pid1, pid2, pid3, n;
    char giorno[DIM_GIORNO], path[DIM_PATH], matricola[DIM_MATRICOLA];

    // controllo numero argomenti (2)
    if (argc != 2)
    {
        printf("errore: numero sbagliato di argomenti\n");
        exit(1);
    }

    // controllo che argv[1] sia un path relativo
    if (argv[1][0] == '/')
    {
        printf("errore: path non relativo\n");
        exit(2);
    }

    // controllo che argv[1] sia una directory esistente
    if (opendir(argv[1]) == NULL)
    {
        printf("errore: path non esistente\n");
        exit(3);
    }

    // installo gestore SIGINT
    signal(SIGINT, handler_sigint);

    while (1)
    {
        printf("inserire la matricola del dipendente: ");
        scanf("%s", matricola);

        printf("inserire il giorno (MMGG): ");
        scanf("%s", giorno);

        // controllo che <dir>/<giorno>.log esista
        sprintf(path, "%s/%s.log", argv[1], giorno);
        if (open(path, O_RDONLY) < 0)
        {
            printf("errore: file non esistente\n");
            continue;
        }

        printf("inserire il numero di timbrature da memorizzare: ");
        scanf("%d", &n);

        // controllo su n
        if (n <= 0)
        {
            printf("errore: %d non è un intero positivo\n", n);
            continue;
        }

        // pipe P1 → P2
        if (pipe(p1p2) < 0)
        {
            printf("errore: creazione pipe p1p2\n");
            exit(4);
        }

        /* --- P1: grep matricola --- */
        pid1 = fork();
        if (pid1 < 0)
        {
            printf("errore: creazione figlio p1\n");
            exit(5);
        }
        if (pid1 == 0)
        {
            signal(SIGINT, SIG_DFL);      // gestore di default

            close(p1p2[0]);               // chiudo lato lettura

            close(1);                     // stdout → pipe p1p2
            dup(p1p2[1]);
            close(p1p2[1]);

            execlp("grep", "grep", matricola, path, (char*)0);
            printf("errore: execlp grep\n");
            exit(6);
        }

        /* pipe P2 → P3 */
        if (pipe(p2p3) < 0)
        {
            printf("errore: creazione pipe p2p3\n");
            exit(7);
        }

        /* --- P2: sort -n (ordina per orario HHMM) --- */
        pid2 = fork();
        if (pid2 < 0)
        {
            printf("errore: creazione figlio p2\n");
            exit(8);
        }
        if (pid2 == 0)
        {
            close(p1p2[1]);               //chiudo scrittura p1p2

            close(0);                     // stdin  ← pipe p1p2
            dup(p1p2[0]);
            close(p1p2[0]);

            close(p2p3[0]);               // chiudo lettura p2p3

            close(1);                     // stdout → pipe p2p3
            dup(p2p3[1]);
            close(p2p3[1]);

            execlp("sort", "sort", "-n", (char*)0);
            printf("errore: execlp sort\n");
            exit(9);
        }

        close(p1p2[0]);                   // P0: chiude pipe p1p2
        close(p1p2[1]);

        /* --- P3: tail -n (ultime N timbrature) --- */
        pid3 = fork();
        if (pid3 < 0)
        {
            printf("errore: creazione figlio p3\n");
            exit(11);
        }
        if (pid3 == 0)
        {
            char str_n[10];

            close(p2p3[1]);               // chiudo scrittura p2p3

            close(0);                     // stdin  ← pipe p2p3
            dup(p2p3[0]);
            close(p2p3[0]);

            close(1);                     // stdout → file di output
            open("ultime_presenze.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

            sprintf(str_n, "%d", n);
            execlp("tail", "tail", "-n", str_n, (char*)0);
            printf("errore: execlp tail\n");
            exit(12);
        }

        close(p2p3[0]);                   // P0: chiude pipe p2p3
        close(p2p3[1]);

        /* attendo la terminazione dei tre figli */
        wait(NULL);
        wait(NULL);
        wait(NULL);

        count++;                          // richiesta completata
    }
}
