#include "types.h"
#include "user.h"
#include "param.h"

int main(int argc, char *argv[])
{
    int child_pid = fork();

    for(int i = 0; i < 10; i++){
        if(child_pid == 0){
            printf(1, "CHILD PROCESS %d\n", getpid());
            child_pid = fork();
        }
        else{
            printf(1, "PROCESS %d -> CHILD PROCESS %d\n", getpid(), child_pid);
        }
    }

    printf(1, "Number of system calls invoked: %d\n", get_syscall_count());

    exit();
}