#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    printf("creating soft link hard link and fifo files\n");
    if (symlink("demo", "sf_link") == -1) {
        perror("symlink failed\n");
    }
    if (link("demo", "hf_link") == -1) {
        perror("link failed\n");
    }
    mkfifo("./fifoFile", S_IRUSR | S_IWUSR);
    system("ls -l");
    return 0;
}
