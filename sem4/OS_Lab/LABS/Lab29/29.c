#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {
    pid_t pid = getpid(); // Get the current process ID

    // Get the current scheduling policy
    int policy = sched_getscheduler(pid);
    if (policy == -1) {
        perror("sched_getscheduler failed");
        return 1;
    }

    // Print the current scheduling policy
    printf("Current scheduling policy: ");
    switch (policy) {
    case SCHED_OTHER: printf("SCHED_OTHER\n"); break;
    case SCHED_FIFO: printf("SCHED_FIFO\n"); break;
    case SCHED_RR: printf("SCHED_RR\n"); break;
    default: printf("Unknown policy\n");
    }

    // Set the scheduling policy to SCHED_FIFO
    struct sched_param param;
    param.sched_priority = 1; // Set a priority (valid for SCHED_FIFO and SCHED_RR)

    if (sched_setscheduler(pid, SCHED_FIFO, &param) == -1) {
        perror("sched_setscheduler failed");
        return 1;
    }

    // Verify the updated scheduling policy
    policy = sched_getscheduler(pid);
    if (policy == -1) {
        perror("sched_getscheduler failed");
        return 1;
    }

    printf("Updated scheduling policy: ");
    switch (policy) {
    case SCHED_OTHER: printf("SCHED_OTHER\n"); break;
    case SCHED_FIFO: printf("SCHED_FIFO\n"); break;
    case SCHED_RR: printf("SCHED_RR\n"); break;
    default: printf("Unknown policy\n");
    }

    return 0;
}