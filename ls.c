#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

// Added wildcard matcher below here: match (pattern, name) returns 1 if the name matches the pattern, and 0 otherwise.
// Wildcard * means zero or more of any character

int match(char *pattern, char *name)
{ // The arguments are two strings pattern (requested query, such as *.c), and name (the file name found in the current directoru with dirent)
 
  if (*pattern == '\0')
    return *name == '\0'; // 1: both are exhausted, i.e. full match. 0: pattern is exhausted but the name is not, i.e. no match

  // Current pattern char is '*'
  else if(*pattern == '*'){
    // '*' can match zero characters: skip it and retry
    if(match(pattern + 1, name))
      return 1;
    // '*' can match one character: advance name and retry (if name not empty)
    else if(*name != '\0' && match(pattern, name + 1))
      return 1;
    return 0;
  }
 
  // Current pattern char is a literal: must match exactly
  if(*pattern == *name)
    return match(pattern + 1, name + 1);
 
  return 0;
}


// Added ls_wild(...), a variant on ls(...) meant to deal with arguments that include wildcards *

void
ls_wild(char *pattern)
{
  int fd;
  struct dirent de;
  struct stat st;
  char buf[512];
 
  if((fd = open(".", 0)) < 0){
    printf(2, "ls: cannot open current directory\n");
    return;
  }
 
  while(read(fd, &de, sizeof(de)) == sizeof(de)){ // Enumerate contents of the current directory (files and other directories)
    if(de.inum == 0)
      continue;
 
    // Build a proper NUL-terminated name from the fixed-width dirent field
    char name[DIRSIZ + 1];
    memmove(name, de.name, DIRSIZ);
    name[DIRSIZ] = '\0';
 
    // Skip . and ..
    if(name[0] == '.' && (name[1] == '\0' ||
       (name[1] == '.' && name[2] == '\0')))
      continue;
 
    if(!match(pattern, name))
      continue;
 
    // Stat the matched file for type/inode/size
    buf[0] = '\0';
    strcpy(buf, name);
    if(stat(buf, &st) < 0){
      printf(1, "ls: cannot stat %s\n", buf);
      continue;
    }
 
    printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
  }
 
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;
 
  if(argc < 2){
    ls(".");
    exit();
  }
 
  for(i = 1; i < argc; i++){ // Added this loop to check for wildcards * in the case 1+ arguments are supplied.
    int has_wild = 0;
    char *p;
    for(p = argv[i]; *p; p++){ // Loop through the characters in the argument argv[i] to check for wildcard *
      if(*p == '*'){
        has_wild = 1;
        break;
      }
    }
 
    if(has_wild)
      ls_wild(argv[i]); // Used when wildcards * are detected. The argument with the wildcard is passed as a pattern
    else
      ls(argv[i]);
  }
 
  exit();
}
