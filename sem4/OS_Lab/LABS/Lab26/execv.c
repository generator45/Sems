#include <stdio.h>
#include <unistd.h>

int main() {
    printf("\n(d) Using execv:\n");
    char* args1[] = { "ls", "-Rl", NULL };
    execv("/bin/ls", args1);
    perror("execv failed");

    return 0;
}