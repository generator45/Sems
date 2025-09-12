#include <stdio.h>
#include <unistd.h>

int main() {
    int pid = fork();

    if (pid == 0) {
        // Child process
        printf("Child process ID: %d\n", getpid());
    }
    else {
        // Parent process
        printf("Parent process ID: %d\n", getpid());
    }

    return 0;
}