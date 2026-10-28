#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5  // Dimensione del buffer limitato
#define NUM_MESSAGGI 10 // Numero totale di messaggi da produrre/consumare

// Variabili condivise (modello ad ambiente globale)
int Buff[N];
int indice_inserimento = 0;
int indice_prelievo = 0;

// Dichiarazione dei semafori
sem_t mutex;  // Garantisce la mutua esclusione sul buffer
sem_t vuoto;  // Conta gli slot liberi nel buffer
sem_t pieno;  // Conta gli slot occupati nel buffer

// --- PROCESSO PRODUTTORE ---
void* produttore(void* arg) 
{
    for (int i = 1; i <= NUM_MESSAGGI; i++) 
    {
        int M = i * 10; // "produco(&M)" - Simula la creazione di un dato
        
        sem_wait(&vuoto);  // Attende che ci sia almeno uno slot vuoto
        sem_wait(&mutex);  // Entra in sezione critica per la mutua esclusione
        
        // "inserisco(M, Buff)"
        Buff[indice_inserimento] = M;
        printf("[Produttore] Inserito %d nella posizione %d\n", M, indice_inserimento);
        indice_inserimento = (indice_inserimento + 1) % N; // Gestione circolare
        
        sem_post(&mutex);  // Esce dalla sezione critica
        sem_post(&pieno);  // Segnala che c'è un nuovo messaggio da consumare
        
        sleep(1); // Rallenta l'esecuzione per visualizzare l'output
    }
    pthread_exit(NULL);
}

// --- PROCESSO CONSUMATORE ---
void* consumatore(void* arg) 
{
    for (int i = 1; i <= NUM_MESSAGGI; i++) 
    {
        int M;
        
        sem_wait(&pieno);  // Attende che ci sia almeno un messaggio
        sem_wait(&mutex);  // Entra in sezione critica per la mutua esclusione
        
        // "prelievo(&M, Buff)"
        M = Buff[indice_prelievo];
        printf("  [Consumatore] Prelevato %d dalla posizione %d\n", M, indice_prelievo);
        indice_prelievo = (indice_prelievo + 1) % N; // Gestione circolare
        
        sem_post(&mutex);  // Esce dalla sezione critica
        sem_post(&vuoto);  // Segnala che si è liberato uno slot
        
        // "consumo()" - Simula l'utilizzo del dato
        sleep(2); // Consuma più lentamente di quanto si produce
    }
    pthread_exit(NULL);
}

int main() 
{
    pthread_t thread_prod, thread_cons;

    // Inizializzazione dei semafori (argomenti: indirizzo, pshared=0 per thread, valore iniziale)
    sem_init(&mutex, 0, 1); // mutex.value = 1
    sem_init(&pieno, 0, 0); // pieno.value = 0
    sem_init(&vuoto, 0, N); // vuoto.value = N

    printf("Inizio elaborazione. Capacita' buffer: %d\n\n", N);

    // Creazione dei thread produttore e consumatore
    if (pthread_create(&thread_prod, NULL, produttore, NULL) != 0) 
    {
        perror("Errore creazione thread produttore");
        exit(1);
    }
    if (pthread_create(&thread_cons, NULL, consumatore, NULL) != 0) 
    {
        perror("Errore creazione thread consumatore");
        exit(1);
    }

    // Attesa della terminazione dei thread
    pthread_join(thread_prod, NULL);
    pthread_join(thread_cons, NULL);

    // Distruzione dei semafori
    sem_destroy(&mutex);
    sem_destroy(&pieno);
    sem_destroy(&vuoto);

    printf("\nElaborazione terminata correttamente.\n");
    return 0;
}