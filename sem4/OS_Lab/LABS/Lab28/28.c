#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <errno.h>

int main() {
    // Get the current priority of the process
    int priority = getpriority(PRIO_PROCESS, 0);
    if (priority == -1) {
        perror("getpriority failed");
        return 1;
    }
    printf("Current priority: %d\n", priority);

    // Modify the priority using the nice system call
    int new_priority = nice(5); // Increase the niceness by 5
    if (new_priority == -1 && errno != 0) {
        perror("nice failed");
        return 1;
    }
    printf("New priority after nice(5): %d\n", new_priority);

    // Verify the updated priority
    priority = getpriority(PRIO_PROCESS, 0);
    if (priority == -1) {
        perror("getpriority failed");
        return 1;
    }
    printf("Updated priority: %d\n", priority);

    return 0;
}