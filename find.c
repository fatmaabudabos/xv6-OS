#include "types.h"
#include "user.h"
#include "stat.h" // for stat struct
#include "fs.h" // for dirent

void find(char *path, char *target, int *found) {
    int fd;
    char buf[512], *p;
    struct stat st;
    struct dirent de;
        
    // Open the file, assign the descriptor to fd, and check it.
    if((fd = open(path, 0)) < 0) {
        printf(1, "find: cannot open: %s\n", path);
        return;
    }

    // Try to access the file stat.
    if(fstat(fd, &st) < 0) {
        printf(1, "find: cannot stat: %s\n", path);
        close(fd);
        return;
    }

    // In case the fd refers to a file.
    if(st.type == T_FILE) {
        // To extract the file name from the path.
        char *temp = path + strlen(path);
        while(temp >= path && *temp != '/')
            temp--;
        temp++;
        // Compare the file name to our target.
        if(strcmp(temp, target) == 0) {
            printf(1, "Path to your file: %s\n", path);
            *found = 1;
        }      
        close(fd);
        return;    
    }

    // Only process if it's actually a directory (it might be T_DEV)
    if(st.type != T_DIR) {
        close(fd);
        return;
    }


    // Avoid overflow when copying it to the buffer.
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        printf(1, "find: path too long\n");
        close(fd);
        return;
    }   

    // Append it to the buffer since it is a directory.
    strcpy(buf, path);
    p = buf + strlen(buf);
    if (path[strlen(path) - 1] != '/')
        *p++ = '/';

    // Read the directory content and recursively search for the file.
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        // Skip the null inodes, current working directory, or parent.
        if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;

        // Another directory/file -> append it.
        memmove(p, de.name, DIRSIZ); // dir/file name.
        p[DIRSIZ] = 0; // null terminator \0.

        // Recursively search in the path.
        find(buf, target, found);

        if(*found) {
            close(fd);
            return;
        }
    }
    close(fd);
    return;
}


int main(int argc, char *argv[]) {
    // 3 arguments should be passed: find <path> <filename>
    if (argc != 3) {
        printf(1, "Usage: find <path> <filename>\n");
        exit();
    }

    // flag to check if the file is found or not.
    int found = 0;

    // passing <path> (argv[1]) & <filename>(argv[2]) to find().
    find(argv[1], argv[2], &found);
    
    if (!found)
        printf(1, "File not found.\n");

    exit();
}