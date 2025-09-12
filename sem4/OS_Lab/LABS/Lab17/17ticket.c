#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char* file_path = "ticket.txt";

    // Open the file for writing (create if it doesn't exist, truncate if it does)
    int fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (fd == -1) {
        perror("File open failed");
        exit(1);
    }

    // Initialize the ticket number
    int ticket_number = 10;
    if (write(fd, &ticket_number, sizeof(ticket_number)) == -1) {
        perror("Write failed");
        close(fd);
        exit(1);
    }

    printf("Ticket number initialized to %d\n", ticket_number);

    close(fd);
    return 0;
}