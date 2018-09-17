#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

int main(){
	while(1){
		char cmd[50];
		strcpy(cmd,"./GPS.pl");
		system(cmd);
		return 1;
	}
}
