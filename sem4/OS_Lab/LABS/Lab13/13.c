#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

int main() {
    fd_set readfds;
    struct timeval timeout;
    int retval;
    // to read the return value of select

    // Initialize the file descriptor set
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    // Set the timeout to 10 seconds
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    printf("Waiting for input from STDIN for 10 seconds...\n");

    // Wait for input on STDIN
    retval = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

    if (retval == -1) {
        perror("select");
        exit(1);
    }
    else if (retval) {
        printf("Data is available now.\n");

        // Read and print the input
        char buffer[1024];
        ssize_t bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (bytesRead == -1) {
            perror("read");
            exit(1);
        }
        buffer[bytesRead] = '\0';
        printf("Read data: %s\n", buffer);
        // %zd is used to print ssize_t
    }
    else {
        printf("No data within 10 seconds.\n");
    }

    return 0;
}