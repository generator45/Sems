#include <stdio.h>
#include <unistd.h>

int main() {
    for (int i = 0;i < 10;sleep(1)) printf("%d ", i++);
    return 0;
}