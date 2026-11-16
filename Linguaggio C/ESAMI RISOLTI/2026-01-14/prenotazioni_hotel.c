#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#define DIM_ID_FILIALE 20
#define DIM_DATA 9
#define DIM_PATH 100

static volatile sig_atomic_t count = 0;

void handler_sigint(){
    printf("numero di richieste: %d\n", count);
    exit(0);
}

int main(int argc, char *argv[]){
    int fd, n, pid, p1p2[2], p2p3[2], p3p0[2];
    char id_filiale[DIM_ID_FILIALE], path[DIM_PATH], data[DIM_DATA], buf;

    // controllo che il numero di parametri sia corretto
    if (argc != 2){
        printf("errore: numero sbagliato di parametri\n");
        exit(1);
    }

    // controllo che argv[1] sia un path assoluto
    if (argv[1][0] != '/'){
        printf("errore: <dir> deve essere un path assoluto\n");
        exit(2);
    }

    // controllo che argv[1] esista
    if (opendir(argv[1]) == NULL){
        printf("errore: <dir> non esiste\n");
        exit(3);
    }

    // imposto la gestione di SIGINT
    signal(SIGINT, handler_sigint);

    while(1){
        printf("inserire l'identificativo della filiale dell'hotel: ");
        scanf("%s", id_filiale);

        // controllo che <dir>/<id_filiale>.book esista
        sprintf(path, "%s/%s.book", argv[1], id_filiale);
        fd = open(path, O_RDONLY);
        if (fd < 0){
            printf("errore: il file %s non esiste\n", path);
            continue;
        }
        close(fd);

        printf("inserire la data di interesse (YYYYMMDD): ");
        scanf("%s", data);

        printf("inserire il numero di risultati da visualizzare: ");
        scanf("%d", &n);

        // controllo che n sia un intero positivo
        if (n <= 0) {
            printf("errore: %d non è un intero positivo\n", n);
            continue;
        }

        // creo la pipe p1p2
        if (pipe(p1p2) < 0){
            perror("P0: pipe p1p2");
            exit(4);
        }

        // creo P1
        pid = fork();
        if (pid < 0){
            perror("P0: fork P1");
            exit(5);
        }
        if (pid == 0){
            // imposto la gestione di SIGINT
            signal(SIGINT, SIG_DFL);

            // chiudo la pipe in lettura
            close(p1p2[0]);

            // redirigo stdout sulla pipe
            close(1);
            dup(p1p2[1]);

            // chiudo la pipe in scrittura
            close(p1p2[1]);

            // seleziono solo le informazioni relative alla data inserita
            execlp("grep", "grep", data, path, NULL);

            // se sono qui c'è stato un errore
            perror("P1: execlp");
            exit(6);
        }

        // creo la pipe p2p3
        if (pipe(p2p3) < 0){
            perror("P0: pipe p2p3");
            exit(7);
        }

        // creo P2
        pid = fork();
       if (pid < 0){
            perror("P0: fork P2");
            exit(8);
        }
        if (pid == 0){
            // imposto la gestione di SIGINT
            signal(SIGINT, SIG_DFL);

            // chiudo p1p2 in scrittura
            close(p1p2[1]);

            // chiudo p2p3 in lettura
            close(p2p3[0]);

            // redirigo stdin da p1p2
            close(0);
            dup(p1p2[0]);

            // chiudo p1p2 in lettura
            close(p1p2[0]);

            // redirigo stdout su p2p3
            close(1);
            dup(p2p3[1]);

            // chiudo p2p3 in scrittura
            close(p2p3[1]);

            // ordino le righe in base all'orario (primo campo), in modo crescente
            execlp("sort", "sort", "-n", NULL);

            // se sono qui c'è stato un errore
            perror("P2: execlp");
            exit(9);
        }

        // chiudo pipe p1p2
        close(p1p2[0]);
        close(p1p2[1]);

        // creo la pipe p3p0
        if (pipe(p3p0) < 0){
            perror("P0: pipe p3p0");
            exit(10);
        } 

        // creo P3
        pid = fork();
        if (pid < 0){
            perror("P0: fork P3");
            exit(11);
        }
        if (pid == 0){
            char str_n[4];

            // imposto la gestione di SIGINT
            signal(SIGINT, SIG_DFL);

            // chiudo p2p3 in scrittura
            close(p2p3[1]);

            // redirigo stdin da p2p3
            close(0);
            dup(p2p3[0]);

            // chiudo p2p3 in lettura
            close(p2p3[0]);

            // chiudo p3p0 in lettura
            close(p3p0[0]);

            // redirigo stdout su p3p0
            close(1);
            dup(p3p0[1]);

            // chiudo p3p0 in scrittura
            close(p3p0[1]);

            // visualizzo i primi n risultati
            sprintf(str_n, "%d", n);
            execlp("head", "head", "-n", str_n, NULL);

            // se sono qui c'è stato un errore
            perror("P3: execlp");
            exit(12);
        }

        // chiudo p2p3
        close(p2p3[0]);
        close(p2p3[1]);

        // chiudo p3p0 in scrittura
        close(p3p0[1]);

        // leggo i risultati da p3p0 e li stampo a video
        while (read(p3p0[0], &buf, 1) > 0){
            write(1, &buf, 1);
        }

        // chiudo p3p0 in lettura
        close(p3p0[0]);

        // aspetto la terminazione dei figli
        wait(NULL);
        wait(NULL);
        wait(NULL);

        // incremento il conteggio delle richieste servite
        count++;
    }

    return 0;
}
