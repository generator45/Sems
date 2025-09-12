#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define RECORD_SIZE 50  // Size of each record

// Helper function to lock or unlock a record
void manage_lock(int fd, int record_num, int lock_type) {
    struct flock lock;

    // Initialize the lock structure
    lock.l_type = lock_type;  // F_WRLCK for write lock, F_RDLCK for read lock, F_UNLCK for unlock
    lock.l_whence = SEEK_SET;
    lock.l_start = record_num * RECORD_SIZE;  // Start of the record
    lock.l_len = RECORD_SIZE;                // Length of the record

    // Apply the lock or unlock
    if (fcntl(fd, F_SETLKW, &lock) == -1) {  // Blocking lock
        perror(lock_type == F_UNLCK ? "Unlock failed" : "Lock failed");
        exit(1);
    }

    if (lock_type == F_WRLCK) {
        printf("Write lock acquired on record %d.\n", record_num);
    }
    else if (lock_type == F_RDLCK) {
        printf("Read lock acquired on record %d.\n", record_num);
    }
    else if (lock_type == F_UNLCK) {
        printf("Record %d unlocked.\n", record_num);
    }
}

// Function to write data to a record
void write_record(int fd, int record_num, const char* data) {
    // Lock the record for writing
    manage_lock(fd, record_num, F_WRLCK);

    // Move to the start of the record
    if (lseek(fd, record_num * RECORD_SIZE, SEEK_SET) == -1) {
        perror("lseek failed");
        manage_lock(fd, record_num, F_UNLCK);
        exit(1);
    }

    // Create a buffer to hold the full record
    char buffer[RECORD_SIZE] = { 0 };  // Initialize with null bytes
    strncpy(buffer, data, RECORD_SIZE - 1);  // Copy the data into the buffer

    // Write the full record (50 bytes)
    if (write(fd, buffer, RECORD_SIZE) != RECORD_SIZE) {
        perror("Write failed");
        manage_lock(fd, record_num, F_UNLCK);
        exit(1);
    }
    printf("Data written to record %d: %s\n", record_num, data);

    // Unlock the record
    manage_lock(fd, record_num, F_UNLCK);
}

// Function to read data from a record
void read_record(int fd, int record_num) {
    char buffer[RECORD_SIZE + 1] = { 0 };  // Buffer to hold the record data

    // Lock the record for reading
    manage_lock(fd, record_num, F_RDLCK);

    // Move to the start of the record
    if (lseek(fd, record_num * RECORD_SIZE, SEEK_SET) == -1) {
        perror("lseek failed");
        manage_lock(fd, record_num, F_UNLCK);
        exit(1);
    }

    // Read the data from the record
    if (read(fd, buffer, RECORD_SIZE) != RECORD_SIZE) {
        perror("Read failed");
        manage_lock(fd, record_num, F_UNLCK);
        exit(1);
    }
    buffer[RECORD_SIZE] = '\0';  // Null-terminate the string
    printf("Data read from record %d: %s\n", record_num, buffer);

    // Unlock the record
    manage_lock(fd, record_num, F_UNLCK);
}

int main() {
    const char* file_path = "records.txt";

    // Open the file for reading and writing, create it if it doesn't exist
    int fd = open(file_path, O_RDWR | O_CREAT, 0664);
    if (fd == -1) {
        perror("File open failed");
        exit(1);
    }

    // Initialize the file with three empty records (if the file is empty)
    for (int i = 0; i < 3; i++) {
        char empty_record[RECORD_SIZE] = { 0 };
        write(fd, empty_record, RECORD_SIZE);
    }

    // Example usage
    write_record(fd, 1, "Record 1: Hello, World!");
    write_record(fd, 2, "Record 2: This is a test.");
    write_record(fd, 3, "Record 3: Ayee!");

    read_record(fd, 1);
    read_record(fd, 2);
    read_record(fd, 3);

    close(fd);
    return 0;
}