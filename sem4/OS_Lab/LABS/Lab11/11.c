#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

void append_with_fd(int fd, const char *data)
{
    if (write(fd, data, strlen(data)) == -1)
    {
        perror("write");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }

    // Duplicate the file descriptor using dup
    int fd_dup = dup(fd);
    if (fd_dup == -1)
    {
        perror("dup");
        close(fd);
        exit(1);
    }

    // Duplicate the file descriptor using dup2
    int fd_dup2 = dup2(fd, fd_dup + 1);
    if (fd_dup2 == -1)
    {
        perror("dup2");
        close(fd);
        close(fd_dup);
        exit(1);
    }

    // Duplicate the file descriptor using fcntl
    int fd_fcntl = fcntl(fd, F_DUPFD, fd_dup2 + 1);
    if (fd_fcntl == -1)
    {
        perror("fcntl");
        close(fd);
        close(fd_dup);
        close(fd_dup2);
        exit(1);
    }

    // Append data using the original file descriptor
    append_with_fd(fd, "Original FD\n");

    // Append data using the duplicated file descriptor (dup)
    append_with_fd(fd_dup, "Duplicated FD (dup)\n");

    // Append data using the duplicated file descriptor (dup2)
    append_with_fd(fd_dup2, "Duplicated FD (dup2)\n");

    // Append data using the duplicated file descriptor (fcntl)
    append_with_fd(fd_fcntl, "Duplicated FD (fcntl)\n");

    // Close all file descriptors
    close(fd);
    close(fd_dup);
    close(fd_dup2);
    close(fd_fcntl);

    return 0;
}