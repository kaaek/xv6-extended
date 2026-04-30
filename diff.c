#include "types.h"
#include "user.h"
#include "stat.h"

int readLine(int fd, char *buf, int max){ // Helper for reading differing lines
  int i = 0;
  char c;
  int n;

  while(i < max - 1){         // Read one logical line (or until buffer is full/EOF).
    n = read(fd, &c, 1);
    if(n < 0) return -1;      // read error
    if(n == 0) break;         // EOF, break if one of the files ends (n = 0 means that it reached EOF as EOF is 0)
    buf[i++] = c;             // Append to output string
    if(c == '\n') break;      // new line, stop.
  }

  buf[i] = '\0';              // Add a null character
  return i;                   // 0 means EOF with no data
}

int main(int argc, char *argv[]){
    //Store the name of both files passed as argument 
    char* file1Name;
    char* file2Name;
    if(argc > 3){
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

    // Read files line-by-line and compare corresponding lines.
    char line1[512], line2[512];
    int n1, n2;
    int lineNo = 1;

    while(1){
    n1 = readLine(file1, line1, sizeof(line1));
    n2 = readLine(file2, line2, sizeof(line2));

    if(n1 < 0 || n2 < 0){
        printf(1, "Error: read failed\n");
        exit();
    }

    if(n1 == 0 && n2 == 0){
        break; // both EOF
    }

    // Mark different if lengths differ or any byte differs.
    int same = 1;
    if(n1 != n2){
        same = 0;
    } else {
        int i;
        for(i = 0; i < n1; i++){
        if(line1[i] != line2[i]){
            same = 0;
            break;
        }
        }
    }

    if(!same){
        // Print mismatched line pairs.
        printf(1, "line %d\n", lineNo);

        printf(1, "%s: %s", file1Name, line1);
        if(n1 > 0 && line1[n1 - 1] != '\n')
            printf(1, "\n");

        printf(1, "%s: %s", file2Name, line2);
        if(n2 > 0 && line2[n2 - 1] != '\n')
            printf(1, "\n");
    }

    lineNo++;
    }    

    exit();
}