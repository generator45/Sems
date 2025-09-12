#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    const char* data1 = "1234567890";
    const char* data2 = "abcdefghij";

    // Write the first 10 bytes
    if (write(fd, data1, 10) != 10) {
        perror("write");
        close(fd);
        exit(1);
    }

    // Move the file pointer by 10 bytes
    // checking the return value of lseek
    off_t offset = lseek(fd, 10, SEEK_CUR);
    if (offset == -1) {
        perror("lseek");
        close(fd);
        exit(1);
    }
    printf("File pointer moved to offset: %ld\n", offset);

    // Write the second 10 bytes
    if (write(fd, data2, 10) != 10) {
        perror("write");
        close(fd);
        exit(1);
    }

    close(fd);
    return 0;
}