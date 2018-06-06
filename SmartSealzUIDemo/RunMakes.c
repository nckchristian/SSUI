#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  char *inArg=argv[1];
  int makeCommand=atoi(inArg);
  
  switch(makeCommand){
    case 0:
      system("make FirstSetup");
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
