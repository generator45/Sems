#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        write(STDERR_FILENO, "Usage: ./fileCopy file1 file2\n", 30);
        exit(1);
    }

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1)
    {
        perror("open source file");
        exit(1);
    }

    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dest_fd == -1)
    {
        perror("open destination file");
        close(src_fd);
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = read(src_fd, buffer, BUFFER_SIZE)) > 0)
    {
        if (write(dest_fd, buffer, bytesRead) != bytesRead)
        {
            perror("write");
            close(src_fd);
            close(dest_fd);
            exit(1);
        }
    }

    if (bytesRead == -1)
    {
        perror("read");
    }

    close(src_fd);
    close(dest_fd);

    return 0;
}