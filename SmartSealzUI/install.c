/*
    All contents of this file were written by Brandon Mord 
    bdrmord001@gmail.com
    
    Original owner of git code Bmord01
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	system("sudo apt-get install glade -y");
	system("sudo apt-get install libi2c-dev");
	system("sudo apt-get install at-spi2-core");
	system("sudo perl -MCPAN -e'install \"LWP::Simple\"\'");
	system("sudo pip3 install adafruit-ads1x15");
	system("sudo apt-get upgrade");
	system("sudo apt-get update");
	system("sudo apt-get install libgtk-3-dev");
	//printf("sudo perl -MCPAN -e'install \"LWP::Simple\"\'");
	return 0;
}
