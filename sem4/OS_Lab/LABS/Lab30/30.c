#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void create_daemon() {
    pid_t pid;

    // Fork the process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process exits
        exit(EXIT_SUCCESS);
    }

    // Child process becomes the session leader
    if (setsid() < 0) {
        perror("Failed to create a new session");
        exit(EXIT_FAILURE);
    }

    // Fork again to ensure the daemon cannot acquire a terminal
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process exits
        exit(EXIT_SUCCESS);
    }

    // Set file permissions 
    umask(0);

    // Change the working directory to root so that if the dir where it was run can umonount without any issue
    if (chdir("/") < 0) {
        perror("Failed to change directory to root");
        exit(EXIT_FAILURE);
    }

    // Close standard file descriptors so that program doesnt accidentally access the standard file descriptors of the terminal which this program is no longer part of
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
void start_scheduled_task(int hour, int minutes) {
    while (1) {
        time_t now = time(NULL);
        struct tm* current_time = localtime(&now);

        if (current_time->tm_hour >= hour && current_time->tm_min >= minutes) {
            // Task to execute
            FILE* file = fopen("/tmp/daemon_task.log", "a");
            if (file) {
                fprintf(file, "Hello\nTask executed at %02d:%02d\n", hour, minutes);
                fclose(file);
            }
            break; // Exit after executing the task
        }

        sleep(30); // Sleep for 30 seconds before checking again
    }

}
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage %s <hour> <minutes> \n", argv[0]);
        return 0;
    }
    int hour = atoi(argv[1]);
    int minutes = atoi(argv[2]);
    printf("creating daemon \n");
    create_daemon();
    start_scheduled_task(hour, minutes);

    return 0;
}
