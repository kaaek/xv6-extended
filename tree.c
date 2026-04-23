#include "types.h"
#include "user.h"
#include "stat.h"
#include "fs.h"

void printDirectory(char* path, char* name, int indent){
    //The base case is that path ends with a file (just print the file's name)
    //The recursive case is that the path ends with a folder (print the folder's name and call printDirectory for each file/folder
    //in said folder with the path being updated to include the file/folder contained in the current folder and the indent being
    //incremented)

    //Check whether the path is that of a file or of a folder (a directory) with fstat() which takes as arguments the file 
    //descriptor of the file/folder and the address of the stat struct. The information about the path will be put into the
    //stat struct.
    struct stat* st = (struct stat*) malloc(sizeof(struct stat*));
    int fd = open(path, 0);
    if(fd < 0){
        printf(1, "Error: unable to open %s\n", path);
        exit();
    }
    fstat(fd, st);
    //The type field within the stat struct stores the type of the path (T_DIR if a directory and T_FILE if a file)
    if(st->type == T_FILE){
        //base case (just print the name of the file)
        //apply the indentation
        for(int i = 0; i < indent; i++){
            printf(1, " ");
        }
        printf(1, "%s\n", name);
    }
    else if(st->type == T_DIR){
        //recursive case
        //1)  print the name of the directory
        for(int i = 0; i < indent; i++){
            printf(1, " ");
        }     
        printf(1, "%s\n", name);
        //2) Read the entries within the directory 
        struct dirent de;
        int n;
        while ((n = read(fd, &de, sizeof(de))) == sizeof(de)){
            //skip empty slots (specified by the inum attribute being set to 0) and other invalid files (like . and ..)
            if(de.inum != 0 && strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0){
                //3) Construct the path of each of the subdirectories or files contained in the directory
                char buffer[512];
                strcpy(buffer, path);
                char *p = buffer + strlen(buffer);
                p++;
                *p = '/';
                strcpy(p, de.name);
                //4) Invoke the method itself by passing the newly formed path and the name of the file/directory and the incremented
                // indentation
                printDirectory(buffer, de.name, (indent + 10));
            }
        }
        close(fd);
    }
    else{
        printf(1, "An error has occured\n");
        exit();
    }
}

int main(int argc, char *argv[]){
    if(argc > 1){
        printDirectory(argv[1], argv[1], 0);
    }
    else{
        printDirectory(".", ".", 0);
    }
    exit();
}
