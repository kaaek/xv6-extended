#include "types.h"
#include "user.h"
#include "stat.h"
#include "fcntl.h"

char* createName(char* name, int chunckNum){
    //A new name is generated for the newly created file given the name of the original file and the number of files having been
    //created.
    char* newName = (char*) malloc(sizeof(char)*100);
    int i = 0;
    while (name[i] != '.')
    {
        newName[i] = name[i];
        i++;
    }
    newName[i] = 'a' + chunckNum;
    while (name[i] != '\0')
    {
        newName[i+1] = name[i];
        i++;
    }
    newName[i+1] = '\0';
    return newName;
}

int main(int argc, char *argv[]){
    //Restricting the user from entering less or more than 3 arguments: argv[1] being the name of the file being split and 
    //argv[2] being the number of bytes of each of the created chunks.
    if(argc < 3){
        printf(1, "Error: too few arguments\n");
        exit();
    }
    else if(argc > 3){
        printf(1, "Error: too many arguments\n");
        exit();
    }

    //Opening the file; the program outputs an error and exits upon failure.
    int fd = open(argv[1], 0);
    if(fd < 0){
        printf(1, "Error: unable to open file %s\n", argv[1]);
        exit();
    }

    //The variable charCount is initialized to 0 and is used later on to store the number of characters (hence, the number of bytes)
    //written to the created files
    int charCount = 0;
    //The number of bytes written to the created files cannot exceed the number stored in chunkSize which is equal to the argument 
    //passed by the user to specify the size of each created file
    int chunkSize = atoi(argv[2]);
    //The variable chunkCount is initialized to 0 and is used later on to keep track of the number of files that were created 
    int chunkCount = 0;
    //The variable newFile is used later on as a file descriptor for each of the created files 
    int newFile;

    //The variables n and buffer are used by the function read wich iteratively fills the buffer up until it reaches EOF of the 
    //file given by the user 
    int n;
    char buffer[512];
    
    while((n = read(fd, buffer, sizeof(buffer))) > 0){
        for(int i = 0; i < n; i++){
            //If charCount is equal to 0, that means that no bytes were written to a file, hence no file was yet created. Therefore,
            //a file is created 
            if(charCount == 0){
                //The createName function is invoked to compose the name of the new file given the name of the original file and
                //the number of files already created 
                char* newFileName = createName(argv[1], chunkCount);
                //The new file is created; upon success, the user is notified of the creation of a new file and upon failure 
                //the program outputs an error and exits
                newFile = open(newFileName, O_CREATE | O_RDWR);
                if(newFile < 0){
                    printf(1, "Error: unable to create file %s\n", newFileName);
                    exit();
                }
                printf(1, "Created the file %s successfully\n", newFileName);
                //The variable keeping track of the number of files created ks incremented by one upon the creation of the new file
                chunkCount++;
            }

            //Each iteration of the for loop goes over one character in the buffer where the content of the original file is 
            //stored and this character (byte) is written to the newly created file
            write(newFile, &buffer[i], 1);
            charCount++;

            //Once the number of characters (bytes) is equal to the number of bytes specified by the user charCount is set back to
            //0 and the file descriptor of the previous file is closed 
            if(charCount == chunkSize){
                charCount = 0;
                close(newFile);
            }
        }
    }

    //If charCount is greater than 0 that means that the for loop ended before charCount reached chunkSize, so the file descriptor
    //was never closed and thus needs to be clsoed
    if(charCount > 0){
        close(newFile);
    }
    //The file descriptor of the original file passed by the user is closed 
    close(fd);
}