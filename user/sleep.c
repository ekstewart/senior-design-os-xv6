#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



/**
 * @author Evan Stewart 
 * 
 * @param argc number of token separated arguments
 * @param argv array of string arguments
 *      ex: echo hello world
 *      argc = 3
 *      argv = ["echo", "hello", "world"]
 * */
int main(int argc, char *argv[])
{
    /*
        Inputs: 
        sleep 10
        sleep 0
        sleep -5    // throw error
        sleep       // throw error
        sleep 3 3   // throw error
    */
   int n;
   if(argc==2){
       n = atoi(argv[1]);
       sleep(n);
   }else{
       fprintf(2, "usage: sleep [num]\n");
   }

   exit();
}
