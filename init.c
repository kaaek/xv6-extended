// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUFFLEN 24
char *argv[] = { "sh", 0 };

char *registeredUsers[] = {"root", "user"};
char *registeredPasswords[] = {"toor", "password"};
int numberRegisteredUsers = 2;

int login(char *username, char *password) {
  int i;
  int loggedIn = 0;
  
  for(i = 0 ; i < numberRegisteredUsers ; i++) {
    if(!strcmp(username, registeredUsers[i]) && !strcmp(password, registeredPasswords[i])) {
      loggedIn = 1;
      break;
    }
  }

  return loggedIn;
}

int
main(void)
{
  int pid, wpid, loggedIn;

  if(open("console", O_RDWR) < 0){
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  for(;;){ // Loops forever (init stays alive permanently)
    printf(1, "init: starting sh\n");
    pid = fork(); // for login: fork a process that does the authentication.
    
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }

    if(pid == 0){ // Child process enters this condition
      printf(1, "Default credentials are root:toor or user:password\n");

      while(1){                     // Attempt login here (and keep attempting till it works)
        printf(1, "Username: ");    // Prompt for username
        char *user = (char *)malloc(BUFFLEN);
        user = gets(user , 20);

        //remove enter key from line read i.e. get "root" instead of "root\n"
        switch(user[strlen(user) - 1]) {
          case '\n': case '\r':
          user[strlen(user) - 1] = 0;
        }
        printf(1, "Password: ");    // Prompt for password
        char *pass = (char *)malloc(BUFFLEN);
        pass = gets(pass , 20);

        //remove enter key from line read
        switch(pass[strlen(pass) - 1]) {
          case '\n': case '\r':
          pass[strlen(pass) - 1] = 0;
        }

        loggedIn = login(user, pass);

        if(loggedIn) {
          //opens shell for the user with exec
          printf(1, "Welcome back %s!\n", user);
          exec("sh", argv); // replaces the child process with a shell.
          printf(1, "init: exec login failed\n");
          exit();
        } else {
          printf(1, "Invalid credentials, try again.\n");
        }
      }
    }
    while((wpid=wait()) >= 0 && wpid != pid) // Parent waits for children to terminate
      printf(1, "zombie!\n");
  }
}