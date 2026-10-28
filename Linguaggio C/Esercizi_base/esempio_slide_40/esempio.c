#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

#define N 5          // Dimensione del buffer
#define NUM_MSG 10   // Numero di messaggi da scambiare

typedef int msg;     // Definiamo il tipo "msg" come un semplice intero

// Struttura che risiederà nella memoria condivisa
typedef struct 
{
    msg Buff[N];
    int in;          // Indice di inserimento
    int out;         // Indice di prelievo
    sem_t mutex;     // Garantisce la mutua esclusione (valore = 1)
    sem_t pieno;     // Conta gli elementi nel buffer (valore = 0)
    sem_t vuoto;     // Conta i posti liberi (valore = N)
} SharedData;

// Funzioni di supporto (simulano le azioni del diagramma)
void produco(msg *M, int id) 
{
    *M = id * 100;
    printf("[Produttore] Ho prodotto il messaggio: %d\n", *M);
}

void inserisco(msg M, SharedData *shared) 
{
    shared->Buff[shared->in] = M;
    shared->in = (shared->in + 1) % N;
    printf("[Produttore] -> Inserito nel buffer\n");
}

void prelievo(msg *M, SharedData *shared) 
{
    *M = shared->Buff[shared->out];
    shared->out = (shared->out + 1) % N;
    printf("  [Consumatore] <- Prelevato dal buffer\n");
}

void consumo(msg M) 
{
    printf("  [Consumatore] Ho consumato il messaggio: %d\n", M);
}

int main() 
{
    // 1. Creazione della memoria condivisa tra processi parent/child
    SharedData *shared = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    if (shared == MAP_FAILED) 
    {
        perror("Errore mmap");
        exit(1);
    }

    shared->in = 0;
    shared->out = 0;

    // 2. Inizializzazione dei semafori. 
    // Il secondo argomento '1' indica che il semaforo è condiviso tra processi.
    sem_init(&shared->mutex, 1, 1); // mutex.value = 1
    sem_init(&shared->pieno, 1, 0); // pieno.value = 0
    sem_init(&shared->vuoto, 1, N); // vuoto.value = N

    // 3. Creazione dei processi
    pid_t pid = fork();

    if (pid < 0) 
    {
        perror("Errore fork");
        exit(1);
    } 
    else if (pid == 0) 
    {
        // ==========================================
        // PROCESSO CONSUMATORE C (Processo Figlio)
        // ==========================================
        msg M_cons;
        for (int i = 1; i <= NUM_MSG; i++) 
        {
            sem_wait(&shared->pieno);   // wait(pieno)
            sem_wait(&shared->mutex);   // wait(mutex)
            
            prelievo(&M_cons, shared);  // prelievo(&M, Buff)
            
            sem_post(&shared->mutex);   // signal(mutex)
            sem_post(&shared->vuoto);   // signal(vuoto)
            
            consumo(M_cons);            // consumo(M)
            sleep(2);                   // Consuma lentamente
        }
        exit(0);
    } 
    else 
    {
        // ==========================================
        // PROCESSO PRODUTTORE P (Processo Padre)
        // ==========================================
        msg M_prod;
        for (int i = 1; i <= NUM_MSG; i++) 
        {
            produco(&M_prod, i);        // produco(&M)
            
            sem_wait(&shared->vuoto);   // wait(vuoto)
            sem_wait(&shared->mutex);   // wait(mutex)
            
            inserisco(M_prod, shared);  // inserisco(M, Buff)
            
            sem_post(&shared->mutex);   // signal(mutex)
            sem_post(&shared->pieno);   // signal(pieno)
            
            sleep(1);                   // Produce più velocemente
        }
        
        // Attesa della terminazione del consumatore
        wait(NULL);

        // Pulizia
        sem_destroy(&shared->mutex);
        sem_destroy(&shared->pieno);
        sem_destroy(&shared->vuoto);
        munmap(shared, sizeof(SharedData));
        
        printf("\nElaborazione terminata con successo.\n");
    }

    return 0;
}