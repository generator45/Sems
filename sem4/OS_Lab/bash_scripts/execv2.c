#include <stdio.h>
#include <unistd.h>

int main()
{
    char *args[] = {"/bin/ls", "-l", NULL}; // Command: ls -l
    printf("Before execv\n");

    execv("/bin/ls", args); // Replaces current process with `ls`

    printf("This line will not be executed if execv succeeds\n");
    return 0;
}
