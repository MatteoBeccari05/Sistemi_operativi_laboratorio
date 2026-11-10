/* file: provasleep.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Handler che verrà eseguito se arriva il segnale
void stampa(int signo) 
{
    printf("sono stato risvegliato!\n");
}

int main() 
{
    int k;
    
    // Associa la funzione 'stampa' al segnale SIGUSR1
    signal(SIGUSR1, stampa);
    
    // Si sospende per 1000 secondi
    k = sleep(1000);
    
    // Stampa i secondi rimanenti non "dormiti"
    printf("Valore di k: %d\n", k);
    exit(0);
}