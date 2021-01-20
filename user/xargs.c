#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"


void xargs(char* prog, char *newargv[], int nargc){
    // printf("prog: %s, newargv[0]: %s, newargv[1]: %s, newargv[2]: %s, nargc: %d\n\n", prog, newargv[0], newargv[1], newargv[2],nargc);
    


    char c, *tmp;
    char buf[512] = "";
    tmp = buf;
    // char** tempargv;
    char* tempargv[255];
    // copy over newargv
    int i = 0;
    while(i<nargc){
        tempargv[i] = "";
        // printf("copying %s into tempargv[%d]\n",newargv[i],i);
        strcpy(tempargv[i],newargv[i]);
        i++;
    }
    // printf("prog: %s, newargv[0]: %s, newargv[1]: %s, newargv[2]: %s, nargc: %d\n\n", prog, newargv[0], newargv[1], newargv[2],nargc);
    
    int pid;
    // printf("prog: %s, tempargv[0]: %s, tempargv[1]: %s, tempargv[2]: %s, nargc: %d\n\n", prog, tempargv[0], tempargv[1], tempargv[2],nargc);
    
    while(read(0,&c,1)>0){
        if(c=='\n'){
            *tmp = '\0';
            tempargv[nargc] = buf;
            // printf("tempargv[1]: %s\n",tempargv[1]);
            // tempargv[nargc+1] = "Evan";
            tempargv[nargc+1] = "\0";
            // tempargv[1] = newargv[1];
            
            // tempargv[1] = "hello";
            // tempargv[2] = buf;
            tmp = buf;

            // printf("prog: %s, tempargv[0]: %s, tempargv[1]: %s, tempargv[2]: %s, nargc: %d\n\n", prog, tempargv[0], tempargv[1], tempargv[2], nargc);
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
    // if(argc<2){
    //     fprintf(2,"xargs bad usage: xargs [executable]");
    //     exit();
    // }
    xargs(tempargv[0],tempargv,argc-1);

    exit();
}