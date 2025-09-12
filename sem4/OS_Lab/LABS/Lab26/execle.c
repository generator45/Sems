#include <stdio.h>
#include <unistd.h>

int main() {
    printf("\n(c) Using execle:\n");
    char* env[] = { NULL }; // Environment variables (can be customized)
    execle("/bin/ls", "ls", "-Rl", NULL, env);
    perror("execle failed");

    return 0;
}