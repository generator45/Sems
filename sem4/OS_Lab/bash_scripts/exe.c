#include <stdio.h>
#include <unistd.h>

int main()
{
    char *prog = "/bin/ls";
    int x = 3;
    char *params[] = {"/bin/ls", "-l", "/home", NULL};
    printf("The Beginning\n");
    printf("pid = % d\n", getpid());
    execv(prog, params);
    printf("pid = % d\n", getpid());
    printf("x = %d\n", x);
    printf("The End\n");
}