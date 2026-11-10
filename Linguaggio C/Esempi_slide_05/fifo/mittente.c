#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() 
{
    int fd;
    char msg[10] = "Ciao FIFO";

    // 1. Apre la FIFO in sola scrittura (deve esistere nel file system)
    fd = open("myfifo", O_WRONLY);
    
    if (fd == -1) 
    {
        perror("Errore open. Assicurati che il destinatario abbia creato la FIFO");
        exit(1);
    }

    // 2. Scrive il messaggio nella FIFO
    printf("Invio il messaggio: %s\n", msg);
    write(fd, msg, strlen(msg) + 1);

    // 3. Chiude il file descriptor
    close(fd);

    return 0;
}