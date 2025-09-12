#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

int main()
{
    const char *filepath = "existing_file.txt";

    // Open the file in read-write mode, create if it does not exist
    int fd = open(filepath, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        if (errno == EEXIST)
        {
            printf("File already exists and O_EXCL flag is set.\n");
        }
        else
        {
            perror("open");
        }
        return 1;
    }

    printf("File opened successfully with file descriptor: %d\n", fd);

    // Close the file descriptor
    close(fd);

    return 0;
}