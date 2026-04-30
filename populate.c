// populate.c
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void writefile(char *path, char *content) {
    int fd = open(path, O_CREATE | O_WRONLY);
    if(fd < 0){
        printf(1, "failed to create %s\n", path);
        return;
    }
    write(fd, content, strlen(content));
    close(fd);
}

int main(void) {
    // root-level files
    writefile("readme.txt", "project notes\nTODO: fix bugs\n");
    writefile("log.txt", "boot ok\ninit ok\nwarning: something minor\n");

    // directories
    mkdir("docs");
    mkdir("src");
    mkdir("tmp");
    mkdir("data");

    // docs/
    writefile("docs/notes.txt", "meeting at 10\nbring report\n");
    writefile("docs/todo.txt", "- finish code\n- test cases\n- cleanup\n");

    // src/
    writefile("src/main.c", "int main(){return 0;}\n");
    writefile("src/util.c", "void help(){}\n");
    mkdir("src/lib");
    writefile("src/lib/math.c", "int add(int a,int b){return a+b;}\n");

    // data/
    writefile("data/input1.txt", "42 13 7 99\n");
    writefile("data/input2.txt", "alpha beta gamma\n");

    // tmp/ (messy / uneven)
    writefile("tmp/a.txt", "temp\n");
    writefile("tmp/b.txt", "1234567890\n");
    mkdir("tmp/cache");
    writefile("tmp/cache/c1", "xx\n");
    writefile("tmp/cache/c2", "yy\n");

    // some deeper uneven structure
    mkdir("data/archive");
    mkdir("data/archive/old");
    writefile("data/archive/old/log1", "old log\n");
    writefile("data/archive/log2", "mid log\n");

    printf(1, "filesystem populated\n");
    exit();
}