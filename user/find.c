#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


/**
 * Borrowed from ls.c
 * 
 * */
char*
fmtname(char *path)
{
//   static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

    return p;

    // Return blank-padded name.
    //   if(strlen(p) >= DIRSIZ)
    //     return p;
    //   memmove(buf, p, strlen(p));
    //   memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    //   return buf;
}

/**
 * @author Evan Stewart
 * Takes file path and either "." or ".." 
 * 
 */
void find(char* path, char* filename){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // attain file descriptor of path
  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  // use stat to get information on that file descriptor
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // switch statement based on file type (normal or directory)
  switch(st.type){
  case T_FILE:
    printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    // check to make sure path isnt super long
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    // copy the path onto the buf variable (so as not to modify path)
    strcpy(buf, path);

    // point p to the end of the buf
    p = buf+strlen(buf);

    // Add slash to show new level then increment pointer
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue; 
      // Copy first 14(DIRSIZ) characters from de.name to p
      memmove(p, de.name, DIRSIZ);
      // Set end of p to be null terminator
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      if(st.type==T_DIR && strcmp(fmtname(buf),".")!=0 && strcmp(fmtname(buf),"..")!=0){
          find(buf,filename);
      }else if(st.type==T_FILE && strcmp(filename,fmtname(buf))==0){
          printf("%s\n", buf);
      }
      
    }
    break;
  }
  close(fd);
}

/**
 * @author Evan Stewart 
 * */
int main(int argc, char *argv[])
{
    char* path, *kw;
    if(argc==3){
        if(strcmp(argv[1],".") || strcmp(argv[1],"..")){
            path = argv[1];
            kw = argv[2];
            find(path,kw);
        }else{
            fprintf(1,"Usage: find [./..] [path]\n");    
        }
    }else{
        fprintf(1,"Usage: find [./..] [path]\n");
    }
    exit();
}
