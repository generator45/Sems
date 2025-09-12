#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void write_lock(int fd) {
    struct flock lock;

    // Initialize the lock structure for write lock
    lock.l_type = F_WRLCK;  // Write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;       // Lock from the start of the file
    lock.l_len = 0;         // Lock the entire file

    printf("Acquiring write lock...\n");
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Write lock failed");
        exit(1);
    }
    printf("Write lock acquired. Writing to the file...\n");

    // Write to the file
    const char* data = "Hey Ya!\n";
    if (write(fd, data, strlen(data)) == -1) {
        perror("Write failed");
        exit(1);
    }

    // Release the lock
    lock.l_type = F_UNLCK;  // Unlock
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Unlock failed");
        exit(1);
    }
    printf("Write lock released.\n");
}

void read_lock(int fd) {
    struct flock lock;

    // Initialize the lock structure for read lock
    lock.l_type = F_RDLCK;  // Read lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;       // Lock from the start of the file
    lock.l_len = 0;         // Lock the entire file

    printf("Acquiring read lock...\n");
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Read lock failed");
        exit(1);
    }
    printf("Read lock acquired. Reading from the file...\n");

    // Read from the file
    char buffer[256];
    lseek(fd, 0, SEEK_SET); // Move to the start of the file
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("Read failed");
        exit(1);
    }
    buffer[bytes_read] = '\0'; // Null-terminate the string
    printf("Data read: %s\n", buffer);

    // Release the lock
    lock.l_type = F_UNLCK;  // Unlock
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Unlock failed");
        exit(1);
    }
    printf("Read lock released.\n");
}

int main() {
    const char* file_path = "testfile.txt";

    // Open the file with mandatory locking enabled
    int fd = open(file_path, O_RDWR | O_CREAT | O_TRUNC, 0664);
    if (fd == -1) {
        perror("File open failed");
        exit(1);
    }

    printf("File opened: %s\n", file_path);

    // Perform write lock
    write_lock(fd);

    // Perform read lock
    read_lock(fd);

    close(fd);
    printf("File closed.\n");

    return 0;
}