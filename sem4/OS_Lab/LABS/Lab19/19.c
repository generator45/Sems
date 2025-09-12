#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "running") == 0) {
        printf("Process is running. PID: %d\n", getpid());
        while (1) {
            // Keep the process running
        }
    }
    else if (strcmp(argv[1], "sleeping") == 0) {
        printf("Process is sleeping. PID: %d\n", getpid());
        sleep(60); // Sleep for 60 seconds
    }
    else if (strcmp(argv[1], "stopped") == 0) {
        printf("Process is stopping. PID: %d\n", getpid());
        raise(SIGSTOP); // Stop the process
    }
    else {
        printf("Invalid argument\n");
    }

    return 0;
}