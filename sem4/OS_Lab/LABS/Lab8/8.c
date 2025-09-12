#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        ssize_t start = 0;
        for (ssize_t i = 0; i < bytesRead; i++)
        {
            if (buffer[i] == '\n')
            {
                buffer[i] = '\0';
                printf("%s\n", &buffer[start]);
                start = i + 1;
                // update the start index to the next character
            }
        }
        if (start < bytesRead)
        {
            printf("%s", &buffer[start]);
        }
    }

    if (bytesRead == -1)
    {
        perror("read");
        close(fd);
        exit(1);
    }

    close(fd);
    return 0;
}