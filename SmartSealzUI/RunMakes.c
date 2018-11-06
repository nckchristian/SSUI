/*
  USED ONLY FOR DEVELOPMENT PURPOSES
  Allows developer to chose which sections to re-compile if any editing has been done
  
  
  run command:
    ./{name of exe} {Number Arg}
  Arg options:
    0       = First Ever start up make command
    1       = Making all background exe that create input for UI
    2       = Make the UI itself with any UI changes
    default = Clean the folder up
*/
/*
    All contents of this file were written by Brandon Mord 
    bdrmord001@gmail.com
    
    Original owner of git code Bmord01
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  char *inArg=argv[1];
  int makeCommand=atoi(inArg);
  
  switch(makeCommand){
    case 0:
      system("make install");
      break;
    case 1:
      system("make bg");
      break;
    case 2:
      system("make");
      break;
    default:
      system("make clean");
      break;
  }
  return 0;
}
