#include "types.h"
#include "user.h"
#include "stat.h"

int main(int argc, char *argv[]){
    //Store the name of both files passed as argument 
    char* file1Name;
    char* file2Name;
    if(argc > 2){
        printf(1, "Error: no more than 2 arguments can be passed\n");
        exit();
    }
    else{
        file1Name = argv[1];
        file2Name = argv[2];
    }

    //open both files
    int file1 = open(file1Name, 0);
    if(file1 < 0){
        printf(1, "Error: unable to open %s\n", file1Name);
        exit();
    }
    int file2 = open(file2Name, 0);
    if(file2 < 0){
        printf(1, "Error: unable to open %s\n", file2Name);
        exit();
    }

    //read both files
    char buffer1[512], buffer2[512];
    int n1, n2;
    int i = 0;

    //break if one of the files ends (n = 0 means that it reached EOF as EOF is 0)
    while ((n1 = read(file1, buffer1, sizeof(buffer1)) != 0) || (n1 = read(file1, buffer1, sizeof(buffer1)) != 0))
    {
        
        if(buffer1[i] != buffer2[i]){
            printf(1, "%c", buffer2[i]);
        }
        
        i++;
        if(i > 512){
            i = 0;
        }
    }

    //if only one file ends, continue reading the other and output the content as it differs from the first
    if(n1 != 0){
        while ((n1 = read(file1, buffer1, sizeof(buffer1)) == 0))
        {
            printf("%c", buffer1[i]);
            i++;
            if(i > 512){
                i = 0;
            }
        }
    }
    else if(n2 != 0){
        while ((n2 = read(file2, buffer2, sizeof(buffer2)) == 0))
        {
            printf("%c", buffer2[i]);
            i++;
            if(i > 512){
                i = 0;
            }
        }
    }
    

    exit();
}