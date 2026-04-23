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
    

    exit();
}