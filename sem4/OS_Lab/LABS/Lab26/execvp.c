#include <stdio.h>
#include <unistd.h>

int main() {
    printf("\n(e) Using execvp:\n");
    char* args2[] = { "ls", "-Rl", NULL };
    execvp("ls", args2);
    perror("execvp failed");

    return 0;
}