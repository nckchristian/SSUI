/*!
 * \brief SmartSealz Project
 * \details SmartSealz Development
 * \details Main Glade Source Code
 * \author Brandon Mord
 * \date 2017-2019
 */

/** \defgroup gladeMain Main Glade Source
 * @brief Source Code Controlling Main Interface
 * @{
 */
#include "globalIncludes.h"
#include "main.h"

/*
    All contents of this file were written by Brandon Mord
    bdrmord001@gmail.com

    Original owner of git code Bmord01
*/
void *upTraffic(void *vargp){
	printf("Inside Traffic Thread");
	int trafficPid = system("./traffic.exe &");
	while(trafficPid==-1){
	    usleep(1000000);
	    trafficPid = system("./traffic.exe &");
	}
	return(NULL);
}

void *upHeartBeat(void *vargp){
	printf("Inside Nav Thread");
	int heartbeatPID = system("python3 heartBeats.py &");
	while(heartbeatPID==-1){
	    usleep(1000000);
	    heartbeatPID = system("python3 heartBeats.py &");
	}
	return(NULL);
}

void *upTPO(void *vargp){
	printf("Inside TPO Thread");
	int tpoPID = system("./TPO.exe &");
	while(tpoPID==-1){
	    usleep(1000000);
	    tpoPID = system("python3 heartBeats.py &");
	}
	return(NULL);
}

void *runNav(void *vargp){
    memset(cmd1,'\0',50);
    strcpy(cmd1,"./Nav.exe ");//navigation data connection executible
    strcat(cmd1,destAltim);
    printf("%s\n",cmd1);
    system(cmd1);
    return NULL;
}
/*                     File Reading Thread Functions                      */

void *readGPS(void * arg){
    inGPS = fopen("DataFiles/gpsdata.txt","r");
    if(!inGPS){
        printf("Here GPS");
    }
    else{
        fscanf(inGPS,"%lf %lf %lf %lf %lf %lf %lf %lf %lf",&GPSLat, &GPSLong, &GPSCourse, &GPSGroundSpeed, &ADSBPressure, &ADSBPitch, &ADSBRoll, &ADSBGyroHeading, &ADSBMagHeading);// Read all information from file gpsdata.txt

        sprintf(destGroundS,"%.2lf",GPSGroundSpeed);
        sprintf(destCourse,"%.2lf",GPSCourse);//Write data to char array to be displayed in label

        gtk_label_set_label((GtkLabel *) lblGS,destGroundS);
        gtk_label_set_label((GtkLabel *) lblGPST,destCourse);//Change label to display Info
        fclose(inGPS);
    }
    return (NULL);
}

void *readFile(void * arg){
    inFile = fopen("DataFiles/Nav.txt","r");//Navigation data file
    if(!inFile){
    	printf("HERE NAV\n");
    }
    else{
        fscanf(inFile,"%d %f %d %d",&inAlt,&inPres,&inPitch,&inRoll);//Read in Navigation Data
        fclose(inFile);
    }
    return (NULL);
}

void *readADSB(void * arg){
    inADSB = fopen("DataFiles/traffic.txt","r");//Traffic alert file written by main.cpp running in background
    if(!inADSB){
    	printf("HERE ADSB\n");
    }
    else{
	    int count = 0;
	    char *line;
	    size_t len=0;
	    ssize_t read;
	    while((read = getline(&line,&len,inADSB))!=-1){
		    count++; //count lines in traffic file
	    }
        fclose(inADSB);

        inADSB = fopen("DataFiles/traffic.txt","r");
	    char tail[20];
	    double lat=0.0,lon=0.0;
	    int altADSB=0,track=0,speed=0;
	    int lineCount=0;
	    char storeT[50];
	    char *token;
	    int Loops = 0;
        while(lineCount!=count){
        	//loop through each piece of traffic in the traffic text file
		    token = strtok(fgets(storeT,50,inADSB)," ");
		    while(token !=NULL){
			    //Seperate infromation into fields for later use
			    switch (Loops){
				    case 0:
					    strcpy(tail,token);
					    if(strlen(tail)==0){
						    skip=true;
						    printf("SKIP\n");
						    break;
					    }
					    break;
				    case 1:
					    lat=atof(token);
					    break;
				    case 2:
					    lon=atof(token);
					    break;
				    case 3:
					    altADSB=atoi(token);
					    break;
				    case 4:
					    track=atoi(token);
					    break;
				    case 5:
					    speed=atoi(token);
					    break;
			    }
			    if(skip){
				    printf("SKIP\n");
				    continue;
			    }
			    token = strtok(NULL," ");
			    Loops++;
		    }
		    Loops=0;
		    lineCount++;
		    if(GPSLat>0 && skip==false){
			    //Do distance equation and add traffic to counter if traffic within range
			    dist = CalcDist(lat,lon);
			    if(tenS){
				    if(Range>dist){
					    TrafficCount++;
				    }
			    }
			    else if(fiveS){
				    if(Range>dist){
					    TrafficCount++;
				    }
			    }
			    else if(oneS){
				    if(Range>dist){
					    TrafficCount++;
				    }
			    }
			    else{

			    }
			    if(TrafficCount!=0){
			        if(TrafficCount>prevTrafficC){
			            //Entering Traffic
			            pid=10;
			            hapticThreadStatus = pthread_create(&tid[4],NULL,upHaptic,(void *) &pid);
			            while(hapticThreadStatus != 0){
			                hapticThreadStatus = pthread_create(&tid[4],NULL,upHaptic,(void *) &pid);
			                printf("STUCK\n");
			            }
			        }
				    prevTrafficC=TrafficCount;
				    //printf("SET PREV %d\n",prevTrafficC);
			    }
		    }
		    else{
			    TrafficCount=prevTrafficC;
		    }
        }
        lineCount=0;
        fclose(inADSB);
    }
    return (NULL);
}

void *readHR(void * arg){
    inHR = fopen("DataFiles/heartrate.txt","r");//Heart Rate text file being written by heartrate.py in background
    if(!inHR){
    	printf("HERE HR");
    }
    else{
        fscanf(inHR,"%d",&HR);
        if(prevHR==0 || HR!=0){
        	prevHR=HR;
        }
        if(HR==0){
        	HR=prevHR;
        }
        fclose(inHR);
    }
    return (NULL);
}

void *checkToggles(void* arg){
    //
	//                 Check for any changes in toggle states
	//
    if(bAltA && !bAlt){
    	bAlt=true;
    	bAltC=true;
    }
    else if(!bAltA && bAlt){
    	bAlt=false;
    	bAltC=true;
    }
    if(bPitchA && !bPitch){
    	bPitch=true;
    	bPitchC=true;
    }
    else if(!bPitchA && bPitch){
    	bPitch=false;
    	bPitchC=true;
    }
    if(bRollA && !bRoll){
    	bRoll=true;
    	bRollC=true;
    }
    else if(!bRollA && bRoll){
    	bRoll=false;
    	bRollC=true;
    }
    if(bGSA && !bGS){
    	bGS=true;
    	bGSC=true;
    }
    else if(!bGSA && bGS){
    	bGS=false;
    	bGSC=true;
    }
    if(bGPSTA && !bGPST){
    	bGPST=true;
    	bGPSTC=true;
    }
    else if(!bGPSTA && bGPST){
    	bGPST=false;
    	bGPSTC=true;
    }
    // ================================================================
	// ===            Set Holding Value if toggle state changes     ===
	// ================================================================
    if(bAltC){
    	if(bAlt){
    		holdAlt=inAlt;
    	}
    	else{
    		holdAlt=-1;
    	}
    }
    if(bPitchC){
    	if(bPitch){
    		holdPitch=inPitch;
    	}
    	else{
    		holdPitch=-1;
    	}
    }
    if(bRollC){
    	if(bRoll){
    		holdRoll=inRoll;
    	}
    	else{
    		holdRoll=-1;
    	}
    }
    if(bGSC){
    	if(bGS){
    		holdGS=GPSGroundSpeed;
    	}
    	else{
    		holdGS=-1;
    	}
    }
    if(bGPSTC){
    	if(bGPST){
    		holdCourse = GPSCourse;
    	}
    	else{
    		holdCourse=-1;
    	}
    }
    //reset toggle button changed to false
    bAltC=false;
    bPitchC=false;
    bRollC=false;
    bMagHeadC=false;
    bGSC=false;
    bGPSTC=false;
    return(NULL);
}


/*                     Haptic motor control function                      */

void haptic(int motorSelect, int wave1, int wave2, int wave3)
{
	// Create I2C buses
	int tca, tcaa, drv;
	char *bus = "/dev/i2c-1";
	if((tca = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus 1. \n");
	}
	if((tcaa = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus 2. \n");
	}
	if((drv = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus 3. \n");
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
				return;
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
				return;
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
				return;
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
				return;
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
				return;
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
// ================================================================
// ===                 Update information                       ===
// ===			Uses the timers to trigger screen updates		===
// ===			Runs all of the file reading for updates		===
// ================================================================

//Update gps by running GPS.pl script while connected to stratux wifi
static gboolean _updateGPS(){
	readGPSThread = pthread_create(&fileTID[0],NULL,readGPS,NULL);
	while(readGPSThread != 0){
	    readGPSThread = pthread_create(&fileTID[0],NULL,readGPS,NULL);
	    printf("STUCK\n");
	}
	pthread_join(fileTID[0],NULL);
    return continue_timer;
}
/*
    All contents of this file were written by Brandon Mord
    bdrmord001@gmail.com

    Original owner of git code Bmord01
*/
// ================================================================
// ===					Change Degrees to Radians				===
// ================================================================
double deg2rad(double inDeg){
	return (inDeg* (PI/180));
}
// ================================================================
// ===			Calculate the distance between traffic targets  ===
// ===			Takes in the lat and long of the target         ===
// ===       Compares input to current gps data from perl script===
// ================================================================
double CalcDist(double inLat,double inLong){
	double dLat = deg2rad(inLat-GPSLat);
	double dLon = deg2rad(inLong-GPSLong);
	double a = sin(deg2rad(inLat)) * sin(deg2rad(GPSLat)) + cos(deg2rad(inLat)) * cos(deg2rad(GPSLat)) * cos(dLon);
	double d = acos(a);
	d=d*180/PI;
	d=d*60 * 1.1515;
	//printf("%lf\n",d);
	return d;
}


/*                     Information Update Function                        */
/*                     Cycles every .25 Seconds                           */

static gboolean _update(){
    //system("iostat >> CPUData.txt");
	//system("free -h >> CPUData.txt");

	navThread = pthread_create(&fileTID[4],NULL,runNav,NULL);
	while(navThread != 0){
	    navThread = pthread_create(&fileTID[4],NULL,runNav,NULL);
	}
	pthread_join(fileTID[4],NULL);
    readFileThread=pthread_create(&fileTID[1],NULL,readFile,NULL);
    while(readFileThread!=0){
        readFileThread=pthread_create(&fileTID[1],NULL,readFile,NULL);
        printf("STUCK\n");
    }
    readADSBThread=pthread_create(&fileTID[2],NULL,readADSB,NULL);
    while(readADSBThread!=0){
        readADSBThread=pthread_create(&fileTID[2],NULL,readADSB,NULL);
        printf("STUCK\n");
    }
    readHRThread = pthread_create(&fileTID[3],NULL,readHR,NULL);
    while(readHRThread!=0){
        readHRThread = pthread_create(&fileTID[3],NULL,readHR,NULL);
        printf("STUCK\n");
    }
    int threadJoins;
    for(threadJoins=1;threadJoins<4;threadJoins++){
        pthread_join(fileTID[threadJoins],NULL);
    }

    //Get all states of toggle buttons for holding calculations
    bAltA=gtk_toggle_button_get_active((GtkToggleButton*) tbAlt);
    bPitchA=gtk_toggle_button_get_active((GtkToggleButton*) tbPitch);
    bRollA=gtk_toggle_button_get_active((GtkToggleButton*) tbRoll);
    bGSA=gtk_toggle_button_get_active((GtkToggleButton*) tbGS);
    bGPSTA=gtk_toggle_button_get_active((GtkToggleButton*) tbGPST);

    checkTogglesThread=pthread_create(&toggleTID,NULL,checkToggles,NULL);
    while(checkTogglesThread!=0){
        checkTogglesThread=pthread_create(&toggleTID,NULL,checkToggles,NULL);
        printf("STUCK\n");
    }

    //Prepare all strings to be printed in gtk labels
    sprintf(destAlt,"%d",inAlt);
    testc++;
    sprintf(destPres,"%f",inPres);
    sprintf(destPitch,"%d",inPitch);
    sprintf(destRoll,"%d",inRoll);
    sprintf(destHR,"%d",HR);
    sprintf(destHoldAlt,"%d",holdAlt);
    sprintf(destHoldPitch,"%d",holdPitch);
    sprintf(destHoldRoll,"%d",holdRoll);

    //Delay traffic printing to keep from constant reminders
    if(TrafficCount!=0){
	    sprintf(destTraffic,"%d -In Range- %d",TrafficCount,((int) Range));
	    zeroCount=0;
    }
    else if(TrafficCount==0){
    	if(zeroCount==25){
    		zeroCount=0;
    		sprintf(destTraffic,"...");
    	}
    	zeroCount++;
    }

    //print values in gtk labels
    gtk_label_set_label((GtkLabel *) lblAltitude,destAlt);
    gtk_label_set_label((GtkLabel *) lblPitch,destPitch);
    gtk_label_set_label((GtkLabel *) lblRoll,destRoll);
    gtk_label_set_label((GtkLabel *) lblTraffic,destTraffic);
    gtk_label_set_label((GtkLabel *) lblHR,destHR);
    //close files
    //fclose(inFile);
    pthread_join(toggleTID,NULL);
    //Print warnings if values outside holding value
    //Send warnings to haptic motors if form described in haptic funciton
    if(bAltA){
		//Above Target Alt
    	if(gtk_switch_get_active(Sense1)){
    		if(holdAlt-inAlt > HighSenseAltitudeEB){
    			gtk_label_set_label(warning,"ALTITUDE WARNING IFR");
    			display=true;
    			pid = AboveAlt;
    		}
			//Below Target Alt
    		else if(holdAlt-inAlt < (-1 * HighSenseAltitudeEB)){
    			gtk_label_set_label(warning,"ALTITUDE WARNING IFR");
    			display=true;
    			pid = BelowAlt;
    		}
    	}
    	else{
			//Above Target Alt
    		if(holdAlt-inAlt >LowSenseAltitudeEB){
    			gtk_label_set_label(warning,"ALTITUDE WARNING VFR");
    			display=true;
    			pid = AboveAlt;
    		}
			//Below Target Alt
    		else if(holdAlt-inAlt < (-1 * LowSenseAltitudeEB)){
    			gtk_label_set_label(warning,"ALTITUDE WARNING VFR");
    			display=true;
    			pid=BelowAlt;
    		}
    	}
    }
    if(bPitchA){
    	if(gtk_switch_get_active(Sense2)){
			//Above Target Pitch
    		if(holdPitch-inPitch>HighSensePitchEB){
				gtk_label_set_label(warning,"PITCH WARNING IFR");
				display=true;
				pid=AbovePitch;
    		}
			//Below Target Pitch
    		else if(holdPitch-inPitch< (-1*HighSensePitchEB)){
				gtk_label_set_label(warning,"PITCH WARNING IFR");
				display=true;
				pid=BelowPitch;
    		}
    	}
    	else{
    		if(holdPitch-inPitch>LowSensePitchEB){
				//Above Target Pitch
				gtk_label_set_label(warning,"PITCH WARNING VFR");
				display=true;
				pid=AbovePitch;
    		}
    		else if(holdPitch-inPitch<(-1*LowSensePitchEB)){
				//Below Target Pitch
				gtk_label_set_label(warning,"PITCH WARNING VFR");
				display=true;
				pid=BelowPitch;
    		}
    	}
    }
    if(bRollA){
    	if(gtk_switch_get_active(Sense3)){
			//Above Target Roll
    		if(holdRoll-inRoll>HighSenseRollEB){
    			gtk_label_set_label(warning,"ROLL WARNING IFR");
    			display=true;
    			pid=AboveRoll;
    		}
			//Below Target Roll
    		else if(holdRoll-inRoll<(-1*HighSenseRollEB)){
    			gtk_label_set_label(warning,"ROLL WARNING IFR");
    			display=true;
    			pid=BelowRoll;
    		}
    	}
    	else{
			//Above Target Roll
    		if(holdRoll-inRoll>LowSenseRollEB){
    			gtk_label_set_label(warning,"ROLL WARNING VFR");
    			display=true;
    			pid=AboveRoll;
    		}
			//Below Target Roll
    		else if(holdRoll-inRoll<(-1*LowSenseRollEB)){
    			gtk_label_set_label(warning,"ROLL WARNING VFR");
    			display=true;
    			pid=BelowRoll;
    		}
    	}
    }

    if(bGSA){
    	if(gtk_switch_get_active(Sense4)){
			//Above Target Ground Speed
    		if(holdGS-GPSGroundSpeed >HighSenseGPSGroundS){
    			gtk_label_set_label(warning,"GROUND SPEED WARNING VFR");
    			display=true;
    		}
    		else if(holdGS-GPSGroundSpeed<(-1 * HighSenseGPSGroundS)){
    			gtk_label_set_label(warning,"GROUND SPEED WARNING VFR");
    			display=true;
    		}
    	}
    	else{
			//Above Target Ground Speed
    		if(holdGS-GPSGroundSpeed >LowSenseGPSGroundS){
    			gtk_label_set_label(warning,"GROUND SPEED WARNING VFR");
    			display=true;
    		}
    		else if(holdGS-GPSGroundSpeed<(-1 * LowSenseGPSGroundS)){
    			gtk_label_set_label(warning,"GROUND SPEED WARNING VFR");
    			display=true;
    		}
    	}
		pid=OffGroundSpeed;
    }
    if(bGPSTA){
        if(holdCourse<90.0 && GPSCourse>270.0){
		    GPSCourse= GPSCourse - 360.0;
	    }
	    else if(holdCourse>270.0 && GPSCourse<90.0){
	        GPSCourse = GPSCourse + 360.0;
	    }
    	if(gtk_switch_get_active(Sense5)){
			//Above Target Course
		    printf("%f %f\n",holdCourse,GPSCourse);
    		if(holdCourse-GPSCourse >HighSenseGPSTrack){
    			gtk_label_set_label(warning,"TRACK WARNING VFR");
    			display=true;
    			pid=RightOfTrack;
    		}
    		else if(holdCourse-GPSCourse<(-1 * HighSenseGPSTrack)){
    			gtk_label_set_label(warning,"TRACK WARNING VFR");
    			display=true;
    			pid=LeftOfTrack;
    		}
    	}
    	else{
			//Above Target Course
    		if(holdCourse-GPSCourse >LowSenseGPSTrack){
    			gtk_label_set_label(warning,"TRACK WARNING VFR");
    			display=true;
    			pid=RightOfTrack;
    		}
    		else if(holdCourse-GPSCourse<(-1 * LowSenseGPSTrack)){
    			gtk_label_set_label(warning,"TRACK WARNING VFR");
    			display=true;
    			pid=LeftOfTrack;
    		}
    	}

	}
    //if there is no warning change warning label to nothing
    if(!display){
    	gtk_label_set_label(warning,"...");
		pid=None;
    }

	if(!threadRunning){
		threadRunning=true;
		hapticThreadStatus = pthread_create(&tid[4],NULL,upHaptic,(void *) &pid);
		while(hapticThreadStatus!=0){
		    hapticThreadStatus = pthread_create(&tid[4],NULL,upHaptic,(void *) &pid);
		    printf("STUCK\n");
		}
		printf("\nHaptic Thread Run\n");
	}

    display=false;
    TrafficCount=0;
    skip=false;
    return continue_timer;
}
/*
    All contents of this file were written by Brandon Mord
    bdrmord001@gmail.com

    Original owner of git code Bmord01
*/
/*  Thread Commands */
void *upHaptic(void *vargp){
	int *inPat = (int*) vargp;
	int pattern = *inPat;
	printf("\n\npattern number: %d\n\n",pattern);
  FILE *err = fopen("errorsLog.txt","a");
	switch(pattern){
		case None:
			break;
		case initialize:
      system("date >> errorsLog.txt");
      fprintf(err, "%d",initialize);
			haptic(0,0,0,0);
			return NULL;
		case AboveAlt:
      system("date >> errorsLog.txt");
      fprintf(err, "%d",initialize);
			haptic(7,16,16,16);
			break;
		case BelowAlt:
      system("date >> errorsLog.txt");
      fprintf(err, "%d",initialize);
			haptic(8,12,12,12);
			break;
		case AbovePitch:
      system("date >> errorsLog.txt");
      fprintf(err, "%d",initialize);
			haptic(7,17,0,0);
			break;
		case BelowPitch:
      system("date >> errorsLog.txt");
      fprintf(err, "%d",initialize);
			haptic(8,17,0,0);
			break;
		case AboveRoll:
      system("date >> errorsLog.txt");
      fprintf(err, "%d",initialize);
			haptic(3,17,0,0);
			usleep(10000);
			haptic(2,17,0,0);
			break;
		case BelowRoll:
      system("date >> errorsLog.txt");
      fprintf(err, "%d",initialize);
			haptic(1,17,0,0);
			usleep(10000);
			haptic(4,17,0,0);
			break;
		case OffGroundSpeed:
      system("date >> errorsLog.txt");
      fprintf(err, "%d",initialize);
			haptic(9,17,0,0);
			break;
		case RightOfTrack:
      system("date >> errorsLog.txt");
      fprintf(err, "%d",initialize);
			haptic(5,5,0,0);
			break;
		case LeftOfTrack:
      system("date >> errorsLog.txt");
      fprintf(err, "%d",initialize);
			haptic(6,5,0,0);
			break;
		case 11:
		    break;
	}
  fclose(err);
	usleep(2000000);
	threadRunning=false;
	return(NULL);
}


/*
    All contents of this file were written by Brandon Mord
    bdrmord001@gmail.com

    Original owner of git code Bmord01
*/
/*GTK Button Commands */

void on_btnStart_clicked(){
    gtk_widget_set_sensitive(((GtkWidget*) btnEnd),true);
    gtk_widget_set_sensitive(((GtkWidget*) btnStart),false);
	if(!start_timer)
    {
    	//Start the timer
        g_timeout_add(100,_update, NULL);//Timer for navigation data
        g_timeout_add(1520,_updateGPS,NULL);//Timer for stratux GPS updates
        if(!Background){
            TPOThreadStatus=pthread_create(&tid[0],NULL,upTPO,((void *)tid[0]));
            while(TPOThreadStatus!=0){
                TPOThreadStatus=pthread_create(&tid[0],NULL,upTPO,((void *)tid[0]));
            }
	        trafficThreadStatus=pthread_create(&tid[1],NULL,upTraffic,((void *)tid[1]));
	        while(trafficThreadStatus!=0){
	            trafficThreadStatus=pthread_create(&tid[1],NULL,upTraffic,((void *)tid[1]));
	        }
	        heartbeatThreadStatus=pthread_create(&tid[2],NULL,upHeartBeat,((void *)tid[2]));
	        while(heartbeatThreadStatus!=0){
	            heartbeatThreadStatus=pthread_create(&tid[2],NULL,upHeartBeat,((void *)tid[2]));
	        }
	        Background=true;
        }

        start_timer = TRUE;
        continue_timer = TRUE;
    }
}

void on_btnEnd_clicked(){
	//Pause Timer
	gtk_widget_set_sensitive(((GtkWidget*) btnStart),true);
	gtk_widget_set_sensitive(((GtkWidget*) btnEnd),false);
	continue_timer = FALSE;
    start_timer = FALSE;
}
bool initialized = false;
void on_btnSet_clicked()
{
    //XInitThreads();
	//Set the altimeter data so that the navigation data is accurate to location
	altim = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spAltim));
	sprintf(destAltim,"%d\n",altim);
	char curr[20];
	strcpy(curr,"Current Setting: ");
	strcat(curr, destAltim);
	gtk_label_set_label(lblCurr,curr);
	pid = 0;
	if(!initialized){
	    gtk_widget_set_sensitive(((GtkWidget*) btnStart),true);
    	haptic(0,0,0,0);
    	initialized=true;
	}
}
void on_tbActiveTraffic_clicked(){
	//Change traffic toggle buttons either to enabled or disabled depending on tbTraffic state
	activeS = gtk_toggle_button_get_active(tbActiveTraffic);

	if(activeS){
		gtk_widget_set_sensitive(((GtkWidget*)  tbOneM),true);
		gtk_widget_set_sensitive(((GtkWidget*) tbFiveM),true);
		gtk_widget_set_sensitive(((GtkWidget*) tbTenM),true);
	}
	else{
		gtk_toggle_button_set_active(tbOneM,false);
		gtk_toggle_button_set_active(tbFiveM,false);
		gtk_toggle_button_set_active(tbTenM,false);
		gtk_widget_set_sensitive(((GtkWidget*) tbOneM),false);
		gtk_widget_set_sensitive(((GtkWidget*) tbFiveM),false);
		gtk_widget_set_sensitive(((GtkWidget*) tbTenM),false);
	}
}

void rangeChange(){
	//Set the range to one of the values on the toggle buttons
	oneS=gtk_toggle_button_get_active(tbOneM);
	fiveS=gtk_toggle_button_get_active(tbFiveM);
	tenS= gtk_toggle_button_get_active(tbTenM);

	if(oneS){
		gtk_widget_set_sensitive(((GtkWidget*) tbFiveM),false);
		gtk_widget_set_sensitive(((GtkWidget*) tbTenM),false);
		Range=1.0;
	}
	else if(fiveS){
		gtk_widget_set_sensitive(((GtkWidget*) tbOneM),false);
		gtk_widget_set_sensitive(((GtkWidget*) tbTenM),false);
		Range=5.0;
	}
	else if(tenS){
		gtk_widget_set_sensitive(((GtkWidget*) tbOneM),false);
		gtk_widget_set_sensitive(((GtkWidget*) tbFiveM),false);
		Range=10.0;
	}
	else{
		gtk_widget_set_sensitive(((GtkWidget*) tbOneM),true);
		gtk_widget_set_sensitive(((GtkWidget*) tbFiveM),true);
		gtk_widget_set_sensitive(((GtkWidget*) tbTenM),true);
	}
}

/* GTK Main Setup */

int main(int argc, char *argv[])
{
    XInitThreads();
    GtkBuilder      *builder;
    GtkWidget       *window;

	int version = atoi(argv[1]);
	if(version == 0){
	   HighSensePressureEB=50.0;
	   HighSensePitchEB=15.0;
	   HighSenseRollEB=20.0;
	   HighSenseAltitudeEB=50.0;
	   HighSenseGPSTrack = 5.0;
	   HighSenseGPSGroundS=50.0;
	   LowSensePressureEB=150.0;
	   LowSensePitchEB=15.0;
	   LowSenseRollEB=55.0;
	   LowSenseAltitudeEB=150.0;
	   LowSenseGPSTrack = 15.0;
	   LowSenseGPSGroundS=150.0;
	}
	else{
	  HighSensePressureEB=5.0;
	  HighSensePitchEB=10.0;
	  HighSenseRollEB=10.0;
	  HighSenseAltitudeEB=2.0;
	  HighSenseGPSTrack = 5.0;
	  HighSenseGPSGroundS=2.0;
	  LowSensePressureEB=10.0;
	  LowSensePitchEB=30.0;
	  LowSenseRollEB=30.0;
	  LowSenseAltitudeEB=4.0;
	  LowSenseGPSTrack = 15.0;
	  LowSenseGPSGroundS=5.0;
	}
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/SSUI.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "SSUI"));
    gtk_builder_connect_signals(builder, NULL);


    btnStart=GTK_BUTTON(gtk_builder_get_object(builder,"btnStart"));	//Starting Reading data button assignment to pointer
    btnEnd=GTK_BUTTON(gtk_builder_get_object(builder,"btnEnd"));		//Pausing Reading data button assignment to pointer
    btnSet=GTK_BUTTON(gtk_builder_get_object(builder,"btnSet"));		//Set Altimeter number button assignment to pointer
    lblAltitude=GTK_LABEL(gtk_builder_get_object(builder,"lblVal1"));	//value of altitude label display assignment to pointer
    lblPitch=GTK_LABEL(gtk_builder_get_object(builder,"lblVal3"));		//value of Pitch label display assignment to pointer
    lblRoll=GTK_LABEL(gtk_builder_get_object(builder,"lblVal4"));		//value of Roll label display assignment to pointer
    lblGS = GTK_LABEL(gtk_builder_get_object(builder,"lblVal6"));		//value of GS label display assignment to pointer
    lblGPST = GTK_LABEL(gtk_builder_get_object(builder,"lblVal7"));		//value of GPST label display assignment to pointer
    lblHR =GTK_LABEL(gtk_builder_get_object(builder,"lblVal8"));		//value of HR label display assignment to pointer
    lblCurr =GTK_LABEL(gtk_builder_get_object(builder,"lblCurr"));		//value of Current Warnings label display assignment to pointer
    lblTraffic =GTK_LABEL(gtk_builder_get_object(builder,"lblTraffic"));//value of Traffic label display assignment to pointer
    Sense1 = GTK_SWITCH(gtk_builder_get_object(builder,"Sense1"));		//Change from high sensitivity to low sensitivity
    Sense2 = GTK_SWITCH(gtk_builder_get_object(builder,"Sense2"));		//Change from high sensitivity to low sensitivity
    Sense3 = GTK_SWITCH(gtk_builder_get_object(builder,"Sense3"));		//Change from high sensitivity to low sensitivity
    Sense4 = GTK_SWITCH(gtk_builder_get_object(builder,"Sense4"));		//Change from high sensitivity to low sensitivity
    Sense5 = GTK_SWITCH(gtk_builder_get_object(builder,"Sense5"));		//Change from high sensitivity to low sensitivity
    Sense6 = GTK_SWITCH(gtk_builder_get_object(builder,"Sense6"));		//Change from high sensitivity to low sensitivity
    tbAlt = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"tbAltitude"));	//button to hold value at point of activation for Altitude
    tbPitch = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"tbPitch"));		//button to hold value at point of activation for Pitch
    tbRoll = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"tbRoll"));		//button to hold value at point of activation for Roll
    tbGS = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"tbGS"));			//button to hold value at point of activation for Ground Speed
    tbGPST  = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"tbGPST"));		//button to hold value at point of activation for GPS Track
    tbHR =GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"tbHR"));			//button to hold value at point of activation for Heart Rate
    tbActiveTraffic =GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"tbOnRange"));			//button to hold value at point of activation for Heart Rate
    tbOneM =GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"tbOneM"));			//button to hold value at point of activation for Heart Rate
    tbFiveM =GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"tbFiveM"));			//button to hold value at point of activation for Heart Rate
    tbTenM =GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"tbTenM"));			//button to hold value at point of activation for Heart Rate
    warning = GTK_LABEL(gtk_builder_get_object(builder,"lblWarning"));
	spAltim=GTK_SPIN_BUTTON(gtk_builder_get_object(builder,"sbAltim"));	//Used to allow selection for altimeter reading

	//Connecting widgets to functions so that signals will run functions
	g_signal_connect(btnStart,"clicked",G_CALLBACK (on_btnStart_clicked), NULL);
	g_signal_connect(btnEnd, "clicked", G_CALLBACK (on_btnEnd_clicked),NULL);
	g_signal_connect(btnSet, "clicked", G_CALLBACK(on_btnSet_clicked), NULL);
	g_signal_connect(tbActiveTraffic,"clicked",G_CALLBACK(on_tbActiveTraffic_clicked),NULL);
	g_signal_connect(tbOneM,"clicked",G_CALLBACK(rangeChange),NULL);
	g_signal_connect(tbFiveM,"clicked",G_CALLBACK(rangeChange),NULL);
	g_signal_connect(tbTenM,"clicked",G_CALLBACK(rangeChange),NULL);



	//=========================================================================
	//= Include the CSS Document to control appearance					    ===
	//=========================================================================
	GtkCssProvider *provider;
	provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(provider, "src/style.css",NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();

	g_timeout_add(100,_update,NULL);
	g_timeout_add(1000,_updateGPS,NULL);
    return 0;
}

void *killThreads(void *args){
    //Exit UI
    char endCmd[25];
    memset(endCmd,'\0',25);
	int ADSBThread;
	FILE *perlThread = fopen("DataFiles/perlThread.txt","r");
	fscanf(perlThread,"%d",&ADSBThread);
	fclose(perlThread);
	printf("%d \n",ADSBThread);
	sprintf(endCmd,"kill %d",ADSBThread);
	system(endCmd);
	memset(endCmd,'\0',25);
	int thread1=0,thread2=0;
	FILE *threads = fopen("DataFiles/threads.txt","r");
	fscanf(threads,"%d\n %d\n",&thread1,&thread2);
	sprintf(endCmd,"kill %d",thread1);
	system(endCmd);
	memset(endCmd,'\0',25);
	sprintf(endCmd,"kill %d",thread2);
	system(endCmd);
	memset(endCmd,'\0',25);
	fclose(threads);
	system("rm DataFiles/threads.txt");
	system(endCmd);
	memset(endCmd,'\0',25);
	FILE *pythonT = fopen("DataFiles/pythonThread.txt","r");
	fscanf(pythonT,"%d",&ADSBThread);
	sprintf(endCmd,"kill %d",ADSBThread);
	system(endCmd);
	memset(endCmd,'\0',25);
	fclose(pythonT);
	system("rm DataFiles/pythonThread.txt");
    return NULL;
}

void on_btnClose_clicked(){
	pthread_t killTid;
	if(initialized){
	    killThreadStatus=pthread_create(&killTid,NULL,killThreads,NULL);
	    while(killThreadStatus!=0){
	        killThreadStatus=pthread_create(&killTid,NULL,killThreads,NULL);
	    }
	    pthread_join(killTid,NULL);
	}
	gtk_main_quit();
}

void on_SSUI_destroy(){
	pthread_t killTid;
	pthread_create(&killTid,NULL,killThreads,NULL);
	pthread_join(killTid,NULL);
	sleep(5);
	gtk_main_quit();
}

/** @} */ /* end of gladeMain */
