#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() 
{
    int fd;
    char msg[10];

    // 1. Crea la FIFO nel file system con permessi di lettura/scrittura (0666)
    if (mkfifo("myfifo", 0666) == -1) 
    {
        perror("Errore mkfifo (potrebbe esistere gia')");
    }

    printf("In attesa di un messaggio...\n");

    // 2. Apre la FIFO in sola lettura. 
    // Questa chiamata è bloccante finché un altro processo non apre la FIFO in scrittura
    fd = open("myfifo", O_RDONLY);
    
    if (fd == -1) 
    {
        perror("Errore open");
        exit(1);
    }

    // 3. Legge il messaggio (fino a 10 byte)
    read(fd, msg, sizeof(msg));
    printf("Messaggio ricevuto: %s\n", msg);

    // 4. Chiude il file descriptor
    close(fd);

    // 5. Elimina fisicamente la FIFO dal file system
    unlink("myfifo");

    return 0;
}