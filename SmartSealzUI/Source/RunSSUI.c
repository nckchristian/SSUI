#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
	char cmd[150];
	strcpy(cmd,"./SSUI & ./traffic.exe & ./TPO.exe & python3 heartBeats.py");
	system(cmd);
	return 1;
}
