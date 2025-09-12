#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void write_lock_and_unlock(int fd, int lock_type) {
    struct flock lock = { 0 };

    // Set up the lock or unlock
    lock.l_type = lock_type;  // F_WRLCK for lock, F_UNLCK for unlock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;         // Lock or unlock the entire file
    lock.l_len = 0;           // Lock or unlock the entire file

    if (fcntl(fd, F_SETLKW, &lock) == -1) {  // Blocking lock
        if (lock_type == F_WRLCK) {
            perror("Write lock failed");
        }
        else {
            perror("Unlock failed");
        }
        exit(1);
    }

    if (lock_type == F_WRLCK) {
        printf("Write lock acquired.\n");
    }
    else {
        printf("Write lock released.\n");
    }
}

int main() {
    const char* file_path = "ticket.txt";

    // Open the file for reading and writing
    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        perror("File open failed");
        exit(1);
    }

    // Acquire a write lock
    write_lock_and_unlock(fd, F_WRLCK);

    // Read the current ticket number
    int ticket_number;
    if (read(fd, &ticket_number, sizeof(ticket_number)) == -1) {
        perror("Read failed");
        write_lock_and_unlock(fd, F_UNLCK);
        close(fd);
        exit(1);
    }

    printf("Current ticket number: %d\n", ticket_number);

    // Increment the ticket number
    ticket_number++;
    printf("New ticket number: %d\n", ticket_number);

    // Move the file pointer to the beginning
    lseek(fd, 0, SEEK_SET);

    // Write the updated ticket number back to the file
    if (write(fd, &ticket_number, sizeof(ticket_number)) == -1) {
        perror("Write failed");
        write_lock_and_unlock(fd, F_UNLCK);
        close(fd);
        exit(1);
    }

    // Release the lock
    write_lock_and_unlock(fd, F_UNLCK);

    close(fd);
    printf("Ticket reservation completed successfully.\n");
    return 0;
}