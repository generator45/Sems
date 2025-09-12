#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Parent process (PID: %d) is about to execute another program.\n", getpid());

    // Replace the current process with the "ls" program
    execl("/bin/ls", "ls", "-l", NULL);

    // If execl returns, it means there was an error
    perror("execl failed");
    return 0;
}