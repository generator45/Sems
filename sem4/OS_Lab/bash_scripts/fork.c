#include <stdio.h>
#include <unistd.h>

int main()
{
    int pid;
    printf("PID of parent = %d\n", getpid());
    pid = fork();
    if (pid == 0)
    {
        printf("child gets the returned value: %d\n", pid);
    }
    else
        printf("parent gets the returned value: %d\n", pid);
    // printf("%d\n", pid);
    return 0;
}