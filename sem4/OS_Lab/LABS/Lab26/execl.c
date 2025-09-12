#include <stdio.h>
#include <unistd.h>

int main() {
    printf("\n(a) Using execl:\n");
    execl("/bin/ls", "ls", "-Rl", NULL);
    perror("execl failed");

    return 0;
}