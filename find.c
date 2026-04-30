#include "types.h"
#include "user.h"
#include "stat.h"
#include "fs.h"

//Recursive function that finds the file given its name and given the directory from which it should start its search
void findFile(char* currPath, char* currName, char* name, int showTraversal){

    //Declaring two variables: a pointer of type struct stat called st where the information abou the file/directory will be stored
    //and a file descriptor for said file. If opening the file fails then an error is shown to the user and the program terminates;
    //if not, then we call fstat which will store in st the information of the file/folder
    struct stat* st = (struct stat*) malloc(sizeof(struct stat));
    int fd = open(currPath, 0);

    if(fd < 0){
        printf(1, "Error: Unable to open directory %s\n", currPath);
        exit();
    }
    else{
        fstat(fd, st);
    }
    
    //BASE CASE: If it is a file that is being traversed then we simply check if its name matches the one the user specified 
    if(st->type == T_FILE){
        if(strcmp(currName, name) == 0){
            //Upon success, we output having found the file in the current directory and the program terminates 
            printf(1, "Found file %s in directory %s\n", currName, currPath);
            exit();
        }
        else if(showTraversal){
            //Upon failure, we output having traversed the file ONLY if the user adds the flag -E
            printf(1, "Opened %s in %s\n", currName, currPath);
        }
    }
    else if(st->type == T_DIR){
    //RECURSIVE CASE: If it is a folder (directory) that is being traversed then we traverse the files/folders it contains and call
    //the function again for each one of them.
    //Here, the function read uses the variable de of type struct dirent as the buffer where it stores the information of the
    //file/folder contained in the directory 
        struct dirent de;
        //namebuf is created to store the name of the file/folder and later on manipulate it to turn into a valid directory 
        char namebuf[DIRSIZ+1];

        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            memmove(namebuf, de.name, DIRSIZ);
            namebuf[DIRSIZ] = 0;
            //Empty slots, ., and .. are not included 
            if(de.inum != 0 && strcmp(namebuf, ".") != 0 && strcmp(namebuf, "..") != 0){
                char buffer[512];
                strcpy(buffer, currPath);
                char *p = buffer + strlen(buffer);
                *p++ = '/';
                strcpy(p, namebuf);
                findFile(buffer,namebuf, name, showTraversal);
            }
        }
    }

    close(fd);
}

int main(int argc, char *argv[]){
    //Restricting the user from entering less or more than 4 arguments: argv[1] being the name of the file being searched, 
    //argv[2] being the name of the directory where the search should happen or the flag -E (to output the traversal) and
    //argv[3] being the flag -E in the case where argv[2] is the directory name 
    if(argc > 4){
        printf(1, "Error: too many arguments\n");
        exit();
    }
    else if(argc < 2){
        printf(1, "Error: too few arguments\n");
        exit();
    }

    //The recursive function findFile takes different arguments depending on the number and the nature of the arguments passed in 
    //the terminal alongside the command

    if(argc == 2){
        //if there is only one argument then it has to be the name of the file being searched so the directory where the search 
        //happens is assumed to be the root directory
        findFile(".", ".", argv[1], 0);
    }
    else if(argc == 3 && strcmp(argv[2], "-E") == 0){
        //if there are 2 arguments then the second one could either be the flag or the name of the directory where the search 
        //should happen
        findFile(".", ".", argv[1], 1);
    }
    else if(argc == 3 && *argv[2] == '.'){
        findFile(argv[2], argv[2], argv[1], 0);
    }
    else if(argc == 4 && *argv[2] == '.' && strcmp(argv[3], "-E") == 0){
        //if there are 3 arguments then the second one has to be the name of the directory where the search should happen and the 
        //third the flag
        findFile(argv[2], argv[2], argv[1], 1);
    }
    else{
        //if the format is not respected then the user is given a warning and the program terminates
        printf(1, "Respect the following format: find <file_name> <directory_name> -E");
        exit();
    }
    
    //If the program did not terminate during the recursion then the file is not found so the program outputs failure and exits
    printf(1, "File not found.\n");
    exit();
}