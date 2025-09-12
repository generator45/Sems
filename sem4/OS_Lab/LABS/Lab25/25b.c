#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <name>\n", argv[0]);
        return 0;
    }

    printf("Parent process (PID: %d) is about to execute './a.out'.\n", getpid());

    execl("./a.out", "./a.out", argv[1], NULL);

    perror("execl failed");
    return 0;
}