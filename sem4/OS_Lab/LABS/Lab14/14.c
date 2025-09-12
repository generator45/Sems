#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

void print_file_type(const char* filename) {
    struct stat fileStat;

    if (lstat(filename, &fileStat) == -1) {
        perror("stat");
        exit(1);
    }

    printf("File: %s\n", filename);
    // printf("st_mode: %o\n", fileStat.st_mode); // Print the raw st_mode value in octal

    switch (fileStat.st_mode & S_IFMT) {
    case S_IFREG:
        printf("Type: Regular file\n");
        break;
    case S_IFDIR:
        printf("Type: Directory\n");
        break;
    case S_IFCHR:
        printf("Type: Character device\n");
        break;
    case S_IFBLK:
        printf("Type: Block device\n");
        break;
    case S_IFIFO:
        printf("Type: FIFO (named pipe)\n");
        break;
    case S_IFLNK:
        printf("Type: Symbolic link\n");
        break;
    case S_IFSOCK:
        printf("Type: Socket\n");
        break;
    default:
        printf("Type: Unknown\n");
        break;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    print_file_type(argv[1]);

    return 0;
}