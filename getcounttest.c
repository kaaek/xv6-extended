#include "types.h"
#include "user.h"
#include "param.h"

int main(int argc, char *argv[])
{
    printf(1, "Number of system calls invoked: %d\n", get_syscall_count());

    exit();
}