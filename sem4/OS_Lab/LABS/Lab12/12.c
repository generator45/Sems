#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void print_opening_mode(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    // this flag retrieves the file status flags
    if (flags == -1)
    {
        perror("fcntl");
        exit(1);
    }

    int accessMode = flags & O_ACCMODE;
    switch (accessMode)
    {
    case O_RDONLY:
        printf("File is opened in read-only mode.\n");
        break;
    case O_WRONLY:
        printf("File is opened in write-only mode.\n");
        break;
    case O_RDWR:
        printf("File is opened in read-write mode.\n");
        break;
    default:
        printf("Unknown opening mode.\n");
        break;
    }
}

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

    print_opening_mode(fd);

    close(fd);
    return 0;
}