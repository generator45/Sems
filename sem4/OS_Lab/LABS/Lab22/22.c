#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) is terminating.\n", getpid());
        _exit(0); // Child process exits
    }
    else {
        // Parent process
        printf("Parent process (PID: %d) is sleeping.\n", getpid());
        sleep(20); // Sleep for 20 seconds to create a zombie state
        printf("Parent process (PID: %d) is now calling wait().\n", getpid());
        wait(NULL); // Clean up the child process
        printf("Child process cleaned up.\n");
    }

    return 0;
}