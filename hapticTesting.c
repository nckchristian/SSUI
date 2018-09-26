//#include <gtk/gtk.h>
//#include <glib.h>
#include  <stdio.h>
#include  <math.h>
#include <stdio.h>
#include  <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#define PI 3.14159265359

void haptic(int motorSelect, int wave1, int wave2, int wave3)
{
	// Create I2C buses
	int tca, tcaa, drv;
	char *bus = "/dev/i2c-1";
	if((tca = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	if((tcaa = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	if((drv = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C devices
	ioctl(tca, I2C_SLAVE, 0x20); //Address of TCA I2C Expander
	ioctl(tcaa, I2C_SLAVE, 0x70); //Address of TCAA IC2 Switch
	ioctl(drv, I2C_SLAVE, 0x5A); //Address of selected driver(s)

	char reg[1] = {0x0C};
	char data[1] = {0x01};
	int tcaSet, tcaaSet;
	if (motorSelect == 1) //Lower Left
	{
		tcaSet = 0xFE;
		tcaaSet = 0x01;
	}
	else if (motorSelect == 2) //Upper Left
	{
		tcaSet = 0xFD;
		tcaaSet = 0x02;
	}
	else if (motorSelect == 3) //Lower Right
	{
		tcaSet = 0xFB;
		tcaaSet = 0x04;
	}
	else if (motorSelect == 4) //Upper Right
	{
		tcaSet = 0xF7;
		tcaaSet = 0x08;
	}
	else if (motorSelect == 5) //Left
	{
		tcaSet = 0xFC;
		tcaaSet = 0x03;
	}
	else if (motorSelect == 6) //Right
	{
		tcaSet = 0xF3;
		tcaaSet = 0x0C;
	}
	else if (motorSelect == 7) //Lower
	{
		tcaSet = 0xFA;
		tcaaSet = 0x05;
	}
	else if (motorSelect == 8) //Upper
	{
		tcaSet = 0xF5;
		tcaaSet = 0x0A;
	}
	else if (motorSelect == 9) //All 4
	{
		tcaSet = 0xF0;
		tcaaSet = 0x0F;
	}
	else
	{
		printf("Invalid Motor Selection\n");
	}
	
	if (wave1 == 0) //No wave forms selected - Initialization and calibration of haptic ERM motors
	{
		//configures IO expander TCA for output to all 4 motors for cali
		char config[2] = {0};
		config[0] = 0x03; 
		config[1] = 0xF0; 
		write(tca, config, 2);

		//Sends a high signal to output channel
		config[0] = 0x01; 
		config[1] = 0x0F; 
		write(tca, config, 2);
		
		//Configures i2c switch to connect all 4 i2c lines
		config[0] = 0; 
		config[1] = 0x0F; 
		write(tcaa, config, 2);

		//set rated voltage 2vrms
		config[0] = 0x16; 
		config[1] = 0x53; 
		write(drv, config, 2);
	
		//set overdrive clamp voltage 3.6v peak
		config[0] = 0x17; 
		config[1] = 0xA4; 
		write(drv, config, 2);

		//change mode to autocalibration
		config[0] = 0x01; 
		config[1] = 0x07; 
		write(drv, config, 2);
	
		//set autocaltime to 500ms
		config[0] = 0x1E; 
		config[1] = 0x20; 
		write(drv, config, 2);
		
		//set go bit
		config[0] = 0x0C;
		config[1] = 0x01; 
		write(drv, config, 2);
	
		//polls go bit until it clears to 0
		reg[1] = 0x0C;
		data[1] = 0x01;
		while (data[0] == 0x01)
		{
			write(drv, reg, 1);
			if(read(drv, data, 1) != 1)
			{
				printf("Error : Input/Output error \n");
				exit(1);
			}
		}
	
		//set feedback control register
		config[0] = 0x1A;
		config[1] = 0xB6; 
		write(drv, config, 2);
		
		//set control 1 register
		config[0] = 0x1B;
		config[1] = 0x93; 
		write(drv, config, 2);

		//set control 2 register
		config[0] = 0x1C;
		config[1] = 0xF5; 
		write(drv, config, 2);
	
		//set control 3 register
		config[0] = 0x1D;
		config[1] = 0x80; 
		write(drv, config, 2);

		//set mode to internal trigger
		config[0] = 0x01;
		config[1] = 0x00; 
		write(drv, config, 2);
	
		//set waveform sequence 1 as triple-click waveform
		config[0] = 0x04;
		config[1] = 0x0C; 
		write(drv, config, 2);
	
		//indicator that there is only one waveform that should be played
		config[0] = 0x05;
		config[1] = 0x00; 
		write(drv, config, 2);
	
		//set go bit
		config[0] = 0x0C;
		config[1] = 0x01; 
		write(drv, config, 2);

		//poll go bit until it clears to 0
		reg[1] = 0x0C;
		data[1] = 0x01;
		while (data[0] == 0x01)
		{
			write(drv, reg, 1);
			if(read(drv, data, 1) != 1)
			{
				printf("Error : Input/Output error \n");
				exit(1);
			}
		}
	
		//deassert the EN pin for driver 1
		config[0] = 0x00;
		config[1] = 0x00; 
		write(tca, config, 2);

		//no driver i2c channels connected
		config[0] = 0;
		config[1] = 0x00; 
		write(tcaa, config, 2);
	}
	else if (wave1 != 0 && wave2 == 0) // one waveform selected
	{
		//configures IO expander TCA for output port at channel x
		char config[2] = {0};
		config[0] = 0x03; 
		config[1] = tcaSet; 
		write(tca, config, 2);
	
		//Configures i2c switch to connect channel x i2c lines
		config[0] = 0; 
		config[1] = tcaaSet; 
		write(tcaa, config, 2);
		
		//set waveform sequence 1
		config[0] = 0x04;
		config[1] = wave1; 
		write(drv, config, 2);
	
		//indicator that there is only one waveform that should be played
		config[0] = 0x05;
		config[1] = 0x00; 
		write(drv, config, 2);
	
		//set go bit
		config[0] = 0x0C;
		config[1] = 0x01; 
		write(drv, config, 2);

		//poll go bit until it clears to 0
		reg[1] = 0x0C;
		data[1] = 0x01;
		while (data[0] == 0x01)
		{
			write(drv, reg, 1);
			if(read(drv, data, 1) != 1)
			{
				printf("Error : Input/Output error \n");
				exit(1);
			}
		}
	
		//deassert the EN pin for driver x
		config[0] = 0x00;
		config[1] = 0x00; 
		write(tca, config, 2);
	
		//no driver i2c channels connected
		config[0] = 0;
		config[1] = 0x00; 
		write(tcaa, config, 2);
	}
	else if ((wave1 != 0 && wave2 != 0) && wave3 == 0) //two waveforms selected
	{
		//configures IO expander TCA for output port at channel x
		char config[2] = {0};
		config[0] = 0x03; 
		config[1] = tcaSet; 
		write(tca, config, 2);
	
		//Configures i2c switch to connect channel x i2c lines
		config[0] = 0; 
		config[1] = tcaaSet; 
		write(tcaa, config, 2);
		
		//set waveform sequence 1
		config[0] = 0x04;
		config[1] = wave1; 
		write(drv, config, 2);
	
		//set waveform sequence 2
		config[0] = 0x05;
		config[1] = wave2;
		write(drv, config, 2);
		
		//indicator that there is only two waveform that should be played
		config[0] = 0x06;
		config[1] = 0x00; 
		write(drv, config, 2);
	
		//set go bit
		config[0] = 0x0C;
		config[1] = 0x01; 
		write(drv, config, 2);

		//poll go bit until it clears to 0
		reg[1] = 0x0C;
		data[1] = 0x01;
		while (data[0] == 0x01)
		{
			write(drv, reg, 1);
			if(read(drv, data, 1) != 1)
			{
				printf("Error : Input/Output error \n");
				exit(1);
			}
		}
	
		//deassert the EN pin for driver x
		config[0] = 0x00;
		config[1] = 0x00; 
		write(tca, config, 2);
	
		//no driver i2c channels connected
		config[0] = 0;
		config[1] = 0x00; 
		write(tcaa, config, 2);
	}
	else if ((wave1 != 0 && wave2 != 0) && wave3 != 0) //three waveforms selected
	{
		//configures IO expander TCA for output port at channel x
		char config[2] = {0};
		config[0] = 0x03; 
		config[1] = tcaSet; 
		write(tca, config, 2);
	
		//Configures i2c switch to connect channel x i2c lines
		config[0] = 0; 
		config[1] = tcaaSet; 
		write(tcaa, config, 2);
		
		//set waveform sequence 1
		config[0] = 0x04;
		config[1] = wave1; 
		write(drv, config, 2);

		//set waveform sequence 2
		config[0] = 0x05;
		config[1] = wave2; 
		write(drv, config, 2);

		//set waveform sequence 3
		config[0] = 0x06;
		config[1] = wave3; 
		write(drv, config, 2);
	
		//indicator that there is only three waveform that should be played
		config[0] = 0x07;
		config[1] = 0x00; 
		write(drv, config, 2);
	
		//set go bit
		config[0] = 0x0C;
		config[1] = 0x01; 
		write(drv, config, 2);

		//poll go bit until it clears to 0
		reg[1] = 0x0C;
		data[1] = 0x01;
		while (data[0] == 0x01)
		{
			write(drv, reg, 1);
			if(read(drv, data, 1) != 1)
			{
				printf("Error : Input/Output error \n");
				exit(1);
			}
		}
	
		//deassert the EN pin for driver x
		config[0] = 0x00;
		config[1] = 0x00; 
		write(tca, config, 2);
	
		//no driver i2c channels connected
		config[0] = 0;
		config[1] = 0x00; 
		write(tcaa, config, 2);
	}
}

int main(int argc, char* argv){
    haptic(9,0,0,0);
	int i;
	for(i=0;i<4;i++){
		haptic(argv[1],argc[2],0,0);
		sleep(2);
	}
    return 0;
}
