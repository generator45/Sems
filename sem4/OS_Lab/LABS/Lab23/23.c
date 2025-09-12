#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        sleep(10); // Sleep to ensure the parent terminates first
        printf("Child process (PID: %d) is now an orphan\n", getpid());
    }
    else {
        // Parent process
        printf("Parent process (PID: %d) is terminating.\n", getpid());
        _exit(0); // Parent process exits
    }

    return 0;
}