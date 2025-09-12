#include <stdio.h>
#include <unistd.h>

int main() {
    printf("\n(b) Using execlp:\n");
    execlp("ls", "ls", "-Rl", NULL);
    perror("execlp failed");

    return 0;
}