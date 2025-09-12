#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    //opening a file here for writing
    int fd = open("test.txt", O_CREAT | O_WRONLY, 0777);
    if (fd == -1) {
        printf("Error opening file\n");
        return 1;
    }
    int pid = fork();

    if (pid == 0) {
        // Child process
        // writing to the file
        write(fd, "Hello from child process\n", 26);
    }
    else {
        // Parent process
        // writing to the file
        write(fd, "Hello from parent process\n", 27);
    }
    close(fd);
    return 0;
}