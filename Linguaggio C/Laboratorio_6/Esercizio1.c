#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define N_MAX 10
#define MSG_SIZE 100

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Numero argomenti errato\n");
        exit(1);
    }

    int N = atoi(argv[1]);
    if(N <= 0 || N > N_MAX)
    {
        printf("Errore nunero figli\n");
        exit(2);
    }

    // Array di N pipe: ogni elemento p[i] ha due descrittori (p[i][0] lettura, p[i][1] scrittura)
    int p[N_MAX][2];

    // 2. Creazione delle N pipe da parte del padre
    for(int i = 0; i < N; i++)
    {
        if(pipe(p[i]) < 0)
        {
            printf("Errore creazione pipe");
            exit(3);
        }
    }


    for(int i = 0; i < N; i++)
    {
        int pid = fork();
        if(pid < 0)
        {
            printf("Errore fork\n");
            exit(4);
        }
        else if(pid == 0)
        {
            //codice processo figlio i 
            for(int j = 0; j < N; j++)
            {
                close(p[j][0]);     // Chiudo lettura di tutte le pipe
                if(j != i)
                {
                    close(p[j][1]); // Chiudo scrittura delle pipe degli altri figli
                }
            }

            // Preparo il messaggio contenente il PID del figlio
            char messaggio[MSG_SIZE];
            snprintf(messaggio, sizeof(messaggio), "Hello world! sono il processo: %d", getpid());

            // Scrivo il messaggio sulla i-esima pipe
            if(write(p[i][1], messaggio, strlen(messaggio) + 1) < 0)
            {
                printf("Errore write nel figlio\n");
                close(p[i][1]);
                exit(5);
            }
            close(p[i][1]);
            exit(6);
        }
    }


    // codice padre
    for(int i = 0; i < N; i++)
    {
        // Il padre non scrive sulle pipe, quindi chiude il lato di scrittura
        close(p[i][1]);
        char buffer[MSG_SIZE];
        int r = read(p[i][0], buffer, sizeof(buffer));

        if(r > 0)
        {
            printf("Il padre ha ricevuto %s\n" , buffer);
        }
        else if(r < 0)
        {
            printf("Errore read nel padre\n");
        }
        // Chiudo il lato di lettura dopo aver letto
        close(p[i][0]);
    }

    // Attendo la terminazione di tutti gli N figli per evitare processi zombie
    for (int i = 0; i < N; i++) 
    {
        wait(NULL);
    }

    return 0;
}