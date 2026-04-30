#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAX_BUF 512

// Fill buffer with some pattern
void fill_buffer(char *buf, int size, char c) {
    for (int i = 0; i < size; i++) {
        buf[i] = c;
    }
}

// Create a file with a specific size (in bytes)
void create_file(char *name, int size, char fillChar) {
    int fd = open(name, O_CREATE | O_WRONLY);
    if (fd < 0) {
        printf(1, "Error creating file %s\n", name);
        return;
    }

    char buf[MAX_BUF];
    fill_buffer(buf, MAX_BUF, fillChar);

    int remaining = size;

    while (remaining > 0) {
        int chunk = remaining > MAX_BUF ? MAX_BUF : remaining;
        int written = write(fd, buf, chunk);

        if (written != chunk) {
            printf(1, "Write error in file %s\n", name);
            close(fd);
            return;
        }

        remaining -= written;
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    // Create files of varying sizes
    create_file("small.txt", 100, 'A');
    create_file("medium.txt", 1000, 'B');
    create_file("large.txt", 4000, 'C');
    create_file("xlarge.txt", 8000, 'D');

    printf(1, "Files created successfully.\n");
    exit();
}