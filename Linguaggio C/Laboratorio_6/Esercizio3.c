#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>

static volatile sig_atomic_t count = 0;

void handler_sigusr1(int sig) {
    (void)sig; // Evita warning di parametro inutilizzato
}

void handler_sigint(int sig) {
    (void)sig;
    printf("\nNumero di richieste servite: %d\n", count);
    exit(0);
}

int main(int argc, char *argv[]) 
{
    char id_utente[20], path[256], buffer[256];
    int fd, n, pid1, pid2, pid3;
    int p1p2[2], p2p3[2], p3p0[2];
    ssize_t n_read;

    // 1. Controllo numero di parametri: libri_prestati <dir>
    if (argc != 2) 
    {
        fprintf(stderr, "Errore: numero sbagliato di parametri. Uso: %s <dir>\n", argv[0]);
        exit(1);
    }

    // 2. Controllo che argv[1] sia un percorso assoluto
    if (argv[1][0] != '/') 
    {
        fprintf(stderr, "Errore: <dir> deve essere un path assoluto\n");
        exit(2);
    }

    // 3. Controllo che la directory argv[1] esista
    DIR *dir_ptr = opendir(argv[1]);
    if (dir_ptr == NULL) 
    {
        perror("Errore apertura directory");
        exit(3);
    }
    closedir(dir_ptr); // Chiudo correttamente il puntatore alla directory

    // 4. Imposto la gestione di SIGINT (Ctrl-C) per P0
    signal(SIGINT, handler_sigint);

    while (1) 
    {
        printf("\nInserire id utente (es. U0001): ");
        if (scanf("%19s", id_utente) != 1) {
            continue;
        }

        // Controllo che <dir>/<id_utente>.txt esista
        snprintf(path, sizeof(path), "%s/%s.txt", argv[1], id_utente);
        fd = open(path, O_RDONLY);
        if (fd < 0) 
        {
            printf("Errore: il file %s non esiste\n", path);
            continue;
        }
        close(fd);

        printf("Inserire numero di risultati da visualizzare: ");
        if (scanf("%d", &n) != 1) 
        {
            // Svuoto il buffer di input se l'utente inserisce testo invece di un numero
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Errore: inserire un numero intero valido\n");
            continue;
        }

        // Controllo che n sia un intero positivo
        if (n <= 0) 
        {
            printf("Errore: %d non è un intero positivo\n", n);
            continue;
        }

        // --- CREAZIONE PIPE E PROCESSO P1 (Ordina cronologicamente) ---
        if (pipe(p1p2) < 0) 
        {
            perror("P0: pipe p1p2");
            exit(4);
        }

        pid1 = fork();
        if (pid1 < 0) 
        {
            perror("P0: fork P1");
            exit(5);
        }
        if (pid1 == 0) 
        {
            // Ripristino comportamento default per SIGINT nei figli
            signal(SIGINT, SIG_DFL);

            // Imposto handler per SIGUSR1 e attendo il via libera da P0
            signal(SIGUSR1, handler_sigusr1);
            pause();

            // Chiudo lato lettura della pipe p1p2
            close(p1p2[0]);

            // Redirigo stdout su p1p2[1]
            dup2(p1p2[1], STDOUT_FILENO);
            close(p1p2[1]);

            // Ordino le righe in modo cronologico (primo campo AAAAMMGG)
            execlp("sort", "sort", "-n", path, (char *)NULL);
            perror("P1: execlp sort");
            exit(6);
        }

        // --- CREAZIONE PIPE E PROCESSO P2 (Filtra NON RESTITUITO) ---
        if (pipe(p2p3) < 0) 
        {
            perror("P0: pipe p2p3");
            exit(7);
        }

        pid2 = fork();
        if (pid2 < 0) 
        {
            perror("P0: fork P2");
            exit(8);
        }
        if (pid2 == 0) 
        {
            signal(SIGINT, SIG_DFL);

            // Chiudo estremi non necessari
            close(p1p2[1]);
            close(p2p3[0]);

            // Redirigo stdin su p1p2[0]
            dup2(p1p2[0], STDIN_FILENO);
            close(p1p2[0]);

            // Redirigo stdout su p2p3[1]
            dup2(p2p3[1], STDOUT_FILENO);
            close(p2p3[1]);

            // Seleziono solo i prestiti non ancora restituiti
            execlp("grep", "grep", "NON RESTITUITO", (char *)NULL);
            perror("P2: execlp grep");
            exit(9);
        }

        // Il padre chiude entrambi gli estremi di p1p2 (non più necessari)
        close(p1p2[0]);
        close(p1p2[1]);

        // --- CREAZIONE PIPE E PROCESSO P3 (Seleziona gli N più recenti) ---
        if (pipe(p3p0) < 0) 
        {
            perror("P0: pipe p3p0");
            exit(10);
        }

        pid3 = fork();
        if (pid3 < 0) 
        {
            perror("P0: fork P3");
            exit(11);
        }
        if (pid3 == 0) 
        {
            char n_str[16];
            signal(SIGINT, SIG_DFL);

            // Chiudo estremi non necessari
            close(p2p3[1]);
            close(p3p0[0]);

            // Redirigo stdin su p2p3[0]
            dup2(p2p3[0], STDIN_FILENO);
            close(p2p3[0]);

            // Redirigo stdout su p3p0[1]
            dup2(p3p0[1], STDOUT_FILENO);
            close(p3p0[1]);

            // Prendo le ultime n righe (le più recenti dopo sort -n)
            snprintf(n_str, sizeof(n_str), "%d", n);
            execlp("tail", "tail", "-n", n_str, (char *)NULL);
            perror("P3: execlp tail");
            exit(12);
        }

        // --- CODICE DEL PADRE P0 ---
        // Chiudo tutti gli estremi delle pipe non usati dal padre
        close(p2p3[0]);
        close(p2p3[1]);
        close(p3p0[1]);

        // Invio segnale SIGUSR1 a P1 per avviare la pipeline
        sleep(1);
        kill(pid1, SIGUSR1);

        // 1. Leggo i risultati da P3 PRIMA di chiamare wait() (evita deadlock)
        printf("\n--- Risultati per %s ---\n", id_utente);
        while ((n_read = read(p3p0[0], buffer, sizeof(buffer) - 1)) > 0) 
        {
            buffer[n_read] = '\0';
            printf("%s", buffer);
        }
        close(p3p0[0]);

        // 2. Attendo la terminazione di tutti e 3 i processi figli
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        waitpid(pid3, NULL, 0);

        // Incremento il contatore delle richieste servite
        count++;
    }

    return 0;    
}