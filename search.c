#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    //Restricting the user from entering less or more than 3 arguments: argv[1] being the word being searched 
    //and argv[2] the name of the file
    if(argc < 3){
        printf(1, "Error: too little arguments\n");
        exit();
    }
    else if(argc > 3){
        printf(1, "Error: too many arguments\n");
        exit();
    }
    //Storing the arguments entered by the user (searched word and file name) as well as the length of the word for future use
    char* word = argv[1];
    int wordLength = strlen(word);
    char* fileName = argv[2];

    //Opening the file (and exiting the program as well as outputting an error in case of failure)
    int fd = open(fileName, 0);
    if(fd < 0){
        printf(1, "Error: could not open %s\n", fileName);
        exit();
    }

    //The variables n and buffer are declared to be used with the read function. The function places the bytes read from the file 
    //in the buffer. It also returns the number of bytes read which is stored in n.
    int n;
    char buffer[512];

    //The variables lineCount and wordCount keep track of the line and the placement of the word in the line for each word that is 
    //traversed.
    //The variable checkWord keeps track of the number of contiguous characters that match that of the word being searched 
    int checkWord = 0;
    int lineCount = 1;
    int wordCount = 1;

    //The function read fills the buffer with the content of the file until reaching EOF (represented by 0).
    while ((n = read(fd, buffer, sizeof(buffer))) > 0)
    {
        //Each time the function read fills the buffer, we iterate through the buffer (up until the n-th byte which is the last byte
        //placed in the buffer)
        for(int i = 0; i < n; i++){
            //Each iteration, we check if the character being read matches with the n-th character of the word being searched.
            //Here n is represented by checkWord which is initialized to 0, gets incremented each time a character matches that of 
            //the word being searched, and is set back to 0 if a character does not match. 
            if(buffer[i] == word[checkWord]){
                checkWord++;
            }
            else if(checkWord == wordLength){
                //If checkWord is equal to the length of the word being searched, that means that all the characters of the word
                //being searched matched with a contiguous sequence of characters in the buffer. Thus, the word was found
                //and the program exits upon success 
                printf(1, "\"%s\" found in %s; line %d, word #%d\n", word, fileName, lineCount, wordCount);
                exit();
            }
            else{
                checkWord = 0;
            }

            //For each space, we increment wordCount each time a space is read and we increment line each time \n is read 
            if(buffer[i] == ' '){
                wordCount++;
            }
            else if(buffer[i] == '\n'){
                lineCount++;
                wordCount = 1;
            }  
        }
    }

    //If the word was not found in the buffer then we output having failed to find the word and exit
    printf(1, "\"%s\" is not found in %s\n", word, fileName);
    
    exit();
}
