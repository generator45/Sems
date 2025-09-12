#include <stdio.h>
#include <sched.h>

int main() {
    int max_priority, min_priority;

    // Get the maximum real-time priority
    max_priority = sched_get_priority_max(SCHED_FIFO);
    if (max_priority == -1) {
        perror("sched_get_priority_max failed");
        return 1;
    }

    // Get the minimum real-time priority
    min_priority = sched_get_priority_min(SCHED_FIFO);
    if (min_priority == -1) {
        perror("sched_get_priority_min failed");
        return 1;
    }

    printf("Maximum real-time priority: %d\n", max_priority);
    printf("Minimum real-time priority: %d\n", min_priority);

    return 0;
}
