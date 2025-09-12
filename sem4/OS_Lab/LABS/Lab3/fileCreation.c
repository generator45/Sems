#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
    // Create a file with read and write permissions for the owner
    int fd = creat("new.txt", S_IRUSR | S_IWUSR);
    // S_IRUSR stands for "read permission, owner" and allows the owner of the file to read it.
    // S_IWUSR stands for "write permission, owner" and allows the owner of the file to write to it.
    if (fd == -1)
    {
        perror("creat");
        return 1;
    }

    printf("File descriptor: %d\n", fd);

    // Close the file descriptor
    close(fd);

    return 0;
}