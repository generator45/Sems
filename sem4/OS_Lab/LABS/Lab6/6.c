#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
// size of the buffer used to read and write data

int main()
{
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
    {
        if (write(STDOUT_FILENO, buffer, bytesRead) != bytesRead)
        {
            write(STDERR_FILENO, "Write error\n", 12);
            exit(1);
        }
    }

    if (bytesRead == -1)
    {
        write(STDERR_FILENO, "Read error\n", 11);
        exit(1);
    }

    return 0;
}