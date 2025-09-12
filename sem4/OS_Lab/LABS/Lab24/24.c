#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t child1, child2, child3;

    // Create the first child process
    if ((child1 = fork()) == 0) {
        printf("Child 1 (PID: %d) is running.\n", getpid());
        sleep(5);
        printf("Child 1 (PID: %d) is terminating.\n", getpid());
        _exit(0);
    }

    // Create the second child process
    if ((child2 = fork()) == 0) {
        printf("Child 2 (PID: %d) is running.\n", getpid());
        sleep(10);
        printf("Child 2 (PID: %d) is terminating.\n", getpid());
        _exit(0);
    }

    // Create the third child process
    if ((child3 = fork()) == 0) {
        printf("Child 3 (PID: %d) is running.\n", getpid());
        sleep(15);
        printf("Child 3 (PID: %d) is terminating.\n", getpid());
        _exit(0);
    }

    // Parent process waits for Child 2
    // printf("Parent process (PID: %d) is waiting for Child 2 (PID: %d).\n", getpid(), child2);
    else {
        waitpid(child1, NULL, 0);
        printf("Parent process detected that Child 1 (PID: %d) has terminated.\n", child2);
        waitpid(child2, NULL, 0);
        printf("Parent process detected that Child 2 (PID: %d) has terminated.\n", child2);
        waitpid(child3, NULL, 0);
        printf("Parent process detected that Child 3 (PID: %d) has terminated.\n", child2);

        // pid_t terminated_pid;
        // while ((terminated_pid = wait(NULL)) > 0) {
        //     printf("Parent process detected that Child (PID: %d) has terminated.\n", terminated_pid);
        // }

        printf("Parent process (PID: %d) is terminating.\n", getpid());
    }
    return 0;
}