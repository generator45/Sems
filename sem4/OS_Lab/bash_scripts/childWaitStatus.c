#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int i = 500;
    printf("The Beginning\n");
    int x = fork();
    printf("Hello world\n");
    if (x == 0)
    {
        printf("I am child\n");
        getchar(); // child waits for user input
    }
    else
    {
        int wstat;
        printf("I am parent of process: %d\n", x);
        wait(&wstat); // parent awaits child
    }
    printf("The End\n");
    return 0;
}