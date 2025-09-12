#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

void print_file_info(const char *filename)
{
    struct stat fileStat;
    // stat() returns information about the file pointed to by filename
    // The information is returned in the stat structure

    if (stat(filename, &fileStat) == -1)
    {
        perror("stat");
        exit(1);
    }

    printf("File: %s\n", filename);
    printf("Inode: %ld\n", (long)fileStat.st_ino);
    printf("Number of hard links: %ld\n", (long)fileStat.st_nlink);
    printf("UID: %d\n", fileStat.st_uid);
    printf("GID: %d\n", fileStat.st_gid);
    printf("Size: %ld bytes\n", (long)fileStat.st_size);
    printf("Block size: %ld bytes\n", (long)fileStat.st_blksize);
    printf("Number of blocks: %ld\n", (long)fileStat.st_blocks);
    printf("Time of last access: %s", ctime(&fileStat.st_atime));
    printf("Time of last modification: %s", ctime(&fileStat.st_mtime));
    printf("Time of last change: %s", ctime(&fileStat.st_ctime));
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    print_file_info(argv[1]);

    return 0;
}