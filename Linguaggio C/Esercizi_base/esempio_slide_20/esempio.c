#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX 5 // Capacità massima della mailbox

// Tipo del messaggio
typedef struct 
{
    int id;
    int dato;
} Messaggio;

// Struttura Coda Circolare FIFO
typedef struct 
{
    Messaggio buffer[MAX];
    int testa; // Indice per l'estrazione
    int coda;  // Indice per l'inserimento
} coda;

// Struttura Mailbox globale condivisa
typedef struct 
{
    coda mbx;
    int num_msg;            // Numero corrente di messaggi presenti
    pthread_mutex_t mutex;  // Mutex per garantire l'accesso esclusivo
} mailbox;

// Variabile condivisa globale: shared mailbox M
mailbox M;

// Inserimento nella coda circolare
void inserisci(coda *c, Messaggio m) 
{
    c->buffer[c->coda] = m;
    c->coda = (c->coda + 1) % MAX;
}

// Estrazione dalla coda circolare
void estrai(coda *c, Messaggio *m) 
{
    *m = c->buffer[c->testa];
    c->testa = (c->testa + 1) % MAX;
}

// Inizializzazione della mailbox
void init_mailbox(mailbox *m) 
{
    m->mbx.testa = 0;
    m->mbx.coda = 0;
    m->num_msg = 0;
    pthread_mutex_init(&m->mutex, NULL);
}

// --- Processo/Thread Mittente ---
void* processo_mittente(void* arg) 
{
    int counter = 1;

    for (int i = 0; i < 10; i++) 
    {
        // <crea messaggio m>
        Messaggio m;
        m.id = counter;
        m.dato = counter * 100;
        printf("[Mittente] Creato messaggio ID: %d, Dato: %d\n", m.id, m.dato);

        // Sezione critica per l'accesso a M
        pthread_mutex_lock(&M.mutex);

        if (M.num_msg < MAX) 
        {
            /* send: */
            inserisci(&M.mbx, m);
            M.num_msg++;
            printf("[Mittente] INVIATO msg ID %d (Messaggi in mailbox: %d/%d)\n", m.id, M.num_msg, MAX);
            /* fine send */
            counter++;
        } 
        else 
        {
            printf("[Mittente] Mailbox PIENA (MAX=%d)! Messaggio non inviato al momento.\n", MAX);
        }

        pthread_mutex_unlock(&M.mutex);

        usleep(300000); // Pausa di 300ms tra un ciclo e l'altro
    }

    return NULL;
}

// --- Processo/Thread Destinatario ---
void* processo_destinatario(void* arg) 
{
    for (int i = 0; i < 10; i++) 
    {
        Messaggio m;
        int ricevuto = 0;

        // Sezione critica per l'accesso a M
        pthread_mutex_lock(&M.mutex);

        if (M.num_msg > 0) 
        {
            /* receive: */
            estrai(&M.mbx, &m);
            M.num_msg--;
            ricevuto = 1;
            printf("[Destinatario] RICEVUTO msg ID %d (Messaggi rimanenti: %d/%d)\n", m.id, M.num_msg, MAX);
            /* fine receive */
        } 
        else 
        {
            printf("[Destinatario] Mailbox VUOTA! Nessun messaggio da estrarre.\n");
        }

        pthread_mutex_unlock(&M.mutex);

        if (ricevuto) 
        {
            // <consuma messaggio m>
            printf("[Destinatario] Consumo messaggio ID %d con valore %d\n", m.id, m.dato);
        }

        usleep(500000); // Pausa di 500ms tra una lettura e l'altra
    }

    return NULL;
}

int main() 
{
    pthread_t th_mittente, th_destinatario;

    // Inizializza la mailbox condivisa
    init_mailbox(&M);

    // Creazione dei processi/thread concorrenti
    pthread_create(&th_mittente, NULL, processo_mittente, NULL);
    pthread_create(&th_destinatario, NULL, processo_destinatario, NULL);

    // Attesa del termine
    pthread_join(th_mittente, NULL);
    pthread_join(th_destinatario, NULL);

    pthread_mutex_destroy(&M.mutex);
    return 0;
}