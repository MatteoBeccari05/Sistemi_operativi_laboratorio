#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc != 2) 
    {
        printf("Uso: %s <nomefile>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0)
    {
        perror("Errore creazione file");
    }

    char buffer[256];

    while(1)
    {
        printf("Inserisci testo: ");
        if(fgets(buffer, sizeof(buffer), stdin) == NULL) break;

        if (strcmp(buffer, "fine\n") == 0) break;

        write(fd, buffer, strlen(buffer));
    }
    close(fd);
    return 0;
}