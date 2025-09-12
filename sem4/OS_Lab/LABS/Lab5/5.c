#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

int main()
{
    int counter = 0;
    char filename[20];

    // Create the temp directory if it does not exist

    if (mkdir("temp", 0777) == -1)
    {
        // already exists
        if (errno != EEXIST)
        {
            perror("mkdir");
            exit(1);
        }
    }
    while (1)
    {
        for (int i = 0; i < 5; i++)
        {
            snprintf(filename, sizeof(filename), "temp/file_%d.txt", counter++);
            int fd = creat(filename, S_IRUSR | S_IWUSR);
            if (fd == -1)
            {
                perror("creat");
                exit(1);
            }
            printf("Created file: %s with file descriptor: %d\n", filename, fd);
            close(fd);
        }
        sleep(1); // Sleep for a second to avoid overwhelming the system
    }

    return 0;
}