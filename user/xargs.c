#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"


void xargs(char* prog, char *newargv[], int nargc){
    char c, *tmp;
    char buf[512] = "";
    tmp = buf;
    char* tempargv[255];

    // copy over newargv
    int i = 0;
    while(i<nargc){
        tempargv[i] = "";
        strcpy(tempargv[i],newargv[i]);
        i++;
    }
    int pid;
    
    while(read(0,&c,1)>0){
        if(c=='\n'){
            *tmp = '\0';
            tempargv[nargc] = buf;
            tempargv[nargc+1] = "\0";
            
            tmp = buf;

            pid = fork();
            if(pid==0){
                exec(prog,tempargv);
            }
        }else{
            *tmp++ = c;
        }
    }
}


// echo "hello\nworld" | xargs echo bye
// argv[1] = uprog
int main(int argc, char *argv[]){
    char** tempargv = ++argv;
    if(argc<2){
        fprintf(2,"xargs bad usage: xargs [executable]");
        exit();
    }
    xargs(tempargv[0],tempargv,argc-1);

    exit();
}