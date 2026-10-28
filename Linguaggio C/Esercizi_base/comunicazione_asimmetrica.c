#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_MESSAGGI 5

// Definizione del tipo msg
typedef struct 
{
    int valore;
} msg;

// Funzione di produzione
void produco(msg *m, int step) 
{
    m->valore = step * 10;
    printf("[Produttore %d] Ho prodotto il valore: %d\n", getpid(), m->valore);
}

// Funzione di consumo
void consumo(msg m) 
{
    printf("[Consumatore %d] Ho consumato il valore: %d\n", getpid(), m.valore);
}

// Simulazione di send(dest, M)
void send_msg(int fd, msg m) 
{
    write(fd, &m, sizeof(msg));
}

// Simulazione di receive(&id, &M)
void receive_msg(int fd, pid_t *id, msg *m) {
    read(fd, m, sizeof(msg));
    *id = getppid(); // Identificatore del processo mittente (il padre)
}

int main() 
{
    int pipefd[2];

    // Creazione del canale di comunicazione
    if (pipe(pipefd) == -1) 
    {
        perror("Errore nella creazione della pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) 
    {
        perror("Errore nella fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) 
    {
        // --- Processo Produttore P ---
        close(pipefd[0]); // Chiude il descrittore di lettura

        msg M;
        int fine = 0;
        int count = 1;

        do 
        {
            produco(&M, count);
            send_msg(pipefd[1], M);
            sleep(1); // Simula il tempo di produzione

            count++;
            if (count > NUM_MESSAGGI) 
            {
                fine = 1;
            }
        } while (!fine);

        close(pipefd[1]);
        wait(NULL); // Attende la terminazione del consumatore
    } 
    else 
    {
        // --- Processo Consumatore C ---
        close(pipefd[1]); // Chiude il descrittore di scrittura

        msg M;
        pid_t id;
        int fine = 0;
        int count = 1;

        do 
        {
            receive_msg(pipefd[0], &id, &M);
            printf("[Consumatore %d] Ricevuto da PID %d\n", getpid(), id);
            consumo(M);

            count++;
            if (count > NUM_MESSAGGI) 
            {
                fine = 1;
            }
        } while (!fine);

        close(pipefd[0]);
    }
    return 0;
}