#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <name>\n", argv[0]);
        return 0;
    }

    printf("Hello, %s! This is the executed program.\n", argv[1]);
    return 0;
}