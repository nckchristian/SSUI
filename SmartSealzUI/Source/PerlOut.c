/*
    All contents of this file were written by Brandon Mord 
    bdrmord001@gmail.com
    
    Original owner of git code Bmord01
*/
/*This program runs the perl executible endlessly until stopped*/
#include "genericIncludes.h"
int main(){
    char cmd[50];
	int thread = getpid();
	FILE *threads = fopen("DataFiles/threads.txt","a");
	fprintf(threads,"%d\n",thread);
	fclose(threads);
	strcpy(cmd,"./GPS.pl");
	while(1){
		system(cmd);
		return 1;
	}
}
