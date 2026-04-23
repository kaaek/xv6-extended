#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
  int fileDescriptor, n;
  char buffer[512];

  fileDescriptor = open("sh_history", O_RDONLY);
  /*
   * fcntl.h is a header file that defines the file control options and access modes used by the open system call.
   * O_RDONLY is one of the macros defined in said file.
   */

  if(fileDescriptor < 0){
    printf(2, "history: cannot open sh_history\n");
    exit();
  }
  // `sh_history` file opened at this point, now read its contents into the bufferfer.
  while((n = read(fileDescriptor, buffer, sizeof(buffer))) > 0){    // While there is content,
    if(write(1, buffer, n) != n){                                   // write to the file descriptor 1, i.e. standard out/stdout 
      printf(2, "history: write error\n");                          // Or throw an error to file descriptor 2, i.e. standard error/stderr
      close(fileDescriptor);
      exit();
    }
  }

  if(n < 0)
    printf(2, "history: read error\n");

  close(fileDescriptor);
  exit();
}