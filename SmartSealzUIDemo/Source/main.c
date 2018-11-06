#include "globalIncludes.h"
/*
    All contents of this file were written by Brandon Mord 
    bdrmord001@gmail.com
    
    Original owner of git code Bmord01
*/
/******************************************************************************///////////////////////////////////////////////////

/*****************************************************************************/
//Used in setup of main for Barometric pressure

// ================================================================
// ===                  Reading MS5611                          ===
// ================================================================
unsigned int PROM_read(int DA, char PROM_CMD)
{
	uint16_t ret = 0;
	uint8_t r8b[] = { 0, 0 };

	if (write(DA, &PROM_CMD, 1) != 1){
		printf("read set reg Failed to write to the i2c bus.\n");
	}

	if (read(DA, r8b, 2) != 2){
		printf("Failed to read from the i2c bus.\n");
	}

	ret = r8b[0] * 256 + r8b[1];

	return ret;
}
///////////////////////////////////////////////////////////////////
// ================================================================
// ===                  Reading MS5611                          ===
// ================================================================
long CONV_read(int DA, char CONV_CMD)
{
	long ret = 0;
	uint8_t D[] = { 0, 0, 0 };

	int  h;
	char zero = 0x0;

	if (write(DA, &CONV_CMD, 1) != 1) {
		printf("write reg 8 bit Failed to write to the i2c bus.\n");
	}

	usleep(OSR_4096);

	if (write(DA, &zero, 1) != 1) {
		printf("write reset 8 bit Failed to write to the i2c bus.\n");
	}

	h = read(DA, &D, 3);

	if (h != 3) {
		printf("Failed to read from the i2c bus %d.\n", h);

	}

	ret = D[0] * (unsigned long)65536 + D[1] * (unsigned long)256 + D[2];

	return ret;
}
////////////////////////////////////////////////////////////////////////////////
// ================================================================
// ===                  Reading I2C port for MU6050             ===
// ================================================================
int read_word_2c(int addr)
{
  int val;
  val = wiringPiI2CReadReg8(fd, addr);
  val = val << 8;
  val += wiringPiI2CReadReg8(fd, addr+1);
  if (val >= 0x8000)
    val = -(65536 - val);

  return val;
}
////////////////////////////////////////////////////////////////////////////////
// ================================================================
// ===                  Filtered angle after fusion             ===
// ================================================================
//Returning the angle filtered by sensor fusion
float comp_filter(float prevAngle, float accelAngle, float gyroRate) {
 
  //These two MUST add up to 1, but can be tweeked for tune the filter
  float tConst1 = 0.05;//0.10
  float tConst2 = 0.95;//0.90
  //calculated angle
  float filterAngle = 0.0;
 
  filterAngle = tConst2 * (prevAngle + (gyroRate * dt)) + (tConst1 * accelAngle);
 
  return filterAngle;
}
/*
    All contents of this file were written by Brandon Mord 
    bdrmord001@gmail.com
    
    Original owner of git code Bmord01
*/
////////////////////////////////////////////////////////////////////////////////
// ================================================================
// ===                  Distance Formula                        ===
// ================================================================
double dist(double a, double b)
{
  return sqrt((a*a) + (b*b));
}
////////////////////////////////////////////////////////////////////////////////
// ================================================================
// ===              Calculate Y rotation                        ===
// ================================================================
double get_y_rotation(double x, double y, double z)
{
  double radians;
  radians = atan2(x, dist(y, z));
  return -(radians * (180.0 / M_PI));
}
////////////////////////////////////////////////////////////////////////////////
// ================================================================
// ===              Calculate X rotation                        ===
// ================================================================
double get_x_rotation(double x, double y, double z)
{
  double radians;
  radians = atan2(y, dist(x, z));
  return (radians * (180.0 / M_PI));
}
////////////////////////////////////////////////////////////////////////////////
// ================================================================
// ===                  NO TILT HEADING - UNUSED                ===
// ================================================================
// No tilt compensation
float noTiltCompensate(float YAxis, float XAxis)
{
  float heading = atan2(YAxis, XAxis);
  return heading;
}
////////////////////////////////////////////////////////////////////////////////
// ================================================================
// ===                  CORRECT Angle - 360                     ===
// ================================================================
// Correct angle
float correctAngle(float heading)
{
  if (heading < 0) { heading += 2 * PI; }
  if (heading > 2 * PI) { heading -= 2 * PI; }

  return heading;
}
////////////////////////////////////////////////////////////////////////////////
// ================================================================
// ===                 TILT COMPENSATION - UNUSED               ===
// ================================================================
// Tilt compensation
float tiltCompensate(float YAxis, float XAxis, float ZAxis, float nPitch, float nRoll)
{
  // Tilt compensation
  float Myh, Mxh;
  
  Mxh = XAxis * cos(nPitch) + YAxis * sin(nRoll) * sin(nPitch) - ZAxis * cos(nRoll) * sin(nPitch);
  Myh = YAxis * cos(nRoll) - ZAxis * sin(nRoll);
 
  float heading = atan2(Myh, Mxh);

  //Serial.print("Compensation Returns: "); Serial.println(heading);

  return heading;
}
////////////////////////////////////////////////////////////////////////////////
// ================================================================
// ===                 Print All Data to CMD line               ===
// ================================================================
void PrintData(){
	printf("Temparature : %.2f F", fltd_Temparature* (9/5) + 32);
	printf("  Pressure : %.2f mbar", fltd_Pressure);
	printf("\n\n");
	printf("My acclX : %.4f\n",acclX);
	printf("My acclY : %.4f\n",acclY);
	printf("My acclZ : %.4f\n",acclZ);
	 
	// Output data to screen
	 
	printf("\n");
   printf("My acclX_scaled: %4.4f acclY_scaled: %4.4f acclZ_scaled: %4.4f",
	 		acclX_scaled, acclY_scaled, acclZ_scaled);
   printf("\n");
   printf("My gyroX_scaled: %4.4f gyroZ_scaled: %4.4f gyroZ_scaled: %4.4f\n", 
	 		gyroX_scaled, gyroY_scaled, gyroZ_scaled);  
   printf("\n");
   rotX=get_x_rotation(acclX_scaled, acclY_scaled, acclZ_scaled);
	rotY=get_y_rotation(acclX_scaled, acclY_scaled, acclZ_scaled);
   printf("My X rotation: %4.4f\n", rotX);
   printf("My Y rotation: %4.4f\n", rotY);
   printf("\n");
	//printf("Magnetic field in X-Axis : %d \n", xMag);
	//printf("Magnetic field in Y-Axis : %d \n", yMag);
	//printf("Magnetic field in Z-Axis : %d \n", zMag);
   printf("Test Pitch      : %4.4f\n",pitch);
   printf("Test Roll       : %4.4f\n",roll);
   printf("Test gyroPitch  : %4.4f\n",gyroPitch);
   printf("Test gyroRoll   : %4.4f\n",gyroRoll);
   printf("Test rollRad    : %4.4f\n",rollRad);
   printf("Test pitchRad   : %4.4f\n",pitchRad);
   printf("Test rollDeg    : %4.4f\n",rollDeg);
   printf("Test pitchDeg   : %4.4f\n",pitchDeg);
	printf("Altitude : %.2f ft. Rate of Climb : %d ft/m\n", ((Altitude /.3048)) ,roc);
   printf("\n");
}
/*
    All contents of this file were written by Brandon Mord 
    bdrmord001@gmail.com
    
    Original owner of git code Bmord01
*/
// ================================================================
// ===                 Function to read MPU6050 Variables       ===
// ================================================================
// Example code found at:
/*https://www.sunfounder.com/learn/sensor-kit-v2-0-for-raspberry-pi-b-plus/
		lesson-32-mpu6050-gyro-acceleration-sensor-sensor-kit-v2-0-for-b-plus.html
*/
void readMPU(){
	fd = wiringPiI2CSetup (0x68);
  	wiringPiI2CWriteReg8 (fd,0x6B,0x00);//disable sleep mode 
	acclX = read_word_2c(0x3B);
	acclY = read_word_2c(0x3D);
	acclZ = read_word_2c(0x3F);
	//Scaled Acceleration Data
	acclX_scaled = acclX / 16384.0 ;
   acclY_scaled = acclY / 16384.0;
   acclZ_scaled = acclZ / 16384.0;
	gyroX = read_word_2c(0x43);
	gyroY = read_word_2c(0x45);
	gyroZ = read_word_2c(0x47);
	//Scaled Gyroscopic Data
	gyroX_scaled = gyroX / 131.0;
   gyroY_scaled = gyroY / 131.0;
   gyroZ_scaled = gyroZ / 131.0;
   //Gyro error to correct for
   int gyroXZero=4;
   int gyroYZero=0;
   int gyroZZero=0;

   float initRoll=0.0, initPitch=0.0;
   float degInitRoll=0.0, degInitPitch=0.0;
   float gyroPitch = degInitPitch;
   float gyroRoll  = degInitRoll;
    
   //Rotation rate in deg/sec
	//LSB found on data sheet per sensor's sensetivity
	float LSB = 131;
	rX=((gyroX/LSB) + gyroXZero);//Roll
	rY = ((gyroY / LSB) + gyroYZero) * -1.0; //Pitch
	rZ = ((gyroZ / LSB) + gyroZZero) * -1.0; //Yaw, Use eventually?
	 
	gyroPitch = (gyroPitch + (rY * dt));
	gyroRoll = (gyroRoll + (rX * dt));
	 
    
   rollRad = atan2(acclY, acclZ);
   pitchRad = atan2(acclX, sqrt(acclY * acclY + acclZ * acclZ));
    
   //Filter Pitch
	prevPitch = comp_filter(prevPitch, accelPitch, rY);
	pitch = (int)prevPitch;
 
	//Filter Roll
	prevRoll = comp_filter(prevRoll, accelRoll, rX);
	roll = (int)prevRoll;
}
////////////////////////////////////////////////////////////////////////////////



// ================================================================
// ===                 Main Function                            ===
// ================================================================
int main(int argc, char *argv[])
{
	FILE *outF;
	char *inArg=argv[1];
	float slpMilB=atof(inArg);
	slpMilB=(slpMilB/100)*33.8639;
	SEA_LEVEL_PRESSURE= slpMilB;
	// ===                 MS5611 Portion of Main                   ===
	//Example code found at:
	//https://github.com/devkoriel/Raspi-readMS5611/blob/master/readMS5611.c
	
	//Variables
	int i;
	int fd;
	uint16_t C[7];
	uint32_t D1;
	uint32_t D2;
	char RESET = 0x1E;
	int64_t dT;
	int32_t TEMP;
	int64_t OFF;
	int64_t SENS;
	int32_t P;
	
	////////////////////////////////////////////////////////////////////////////////
	//Opening connection to MS5611
	if ((fd = open("/dev/i2c-1", O_RDWR)) < 0){
		printf("Failed to open the bus.\n");
		//return -1;
	}

	if (ioctl(fd, I2C_SLAVE, MS5611_ADDRESS) < 0){
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//return -1;
	}

	if (write(fd, &RESET, 1) != 1) {
		printf("write reg 8 bit Failed to write to the i2c bus.\n");
	}
	
	for (i = 0; i < 7; i++){
		usleep(1000);

		C[i] = PROM_read(fd, CMD_PROM_READ + (i * 2));
		//printf("C[%d] = %d\n", i, C[i]);
	}
	// ================================================================
	// ===                 While Loop of Main                       ===
	// ================================================================
  	 //clear screen
   	clear();
   	/*
    All contents of this file were written by Brandon Mord 
    bdrmord001@gmail.com
    
    Original owner of git code Bmord01
    */
	/////////////////////////////////////////////////////////////////////////////
	// ================================================================
	// ===                 Reading Information of MS5611            ===
	// ================================================================	 
		clock_gettime(CLOCK_MONOTONIC, &spec);
		curSampled_time = round(spec.tv_nsec / 1.0e6);

		prevSampling_time = Sampling_time;
		Sampling_time = (float)curSampled_time - (float)prevSampled_time;

		if (Sampling_time < 0) // to prevent negative sampling time
			Sampling_time = prevSampling_time;
		D1 = CONV_read(fd, CONV_D1_4096);
		D2 = CONV_read(fd, CONV_D2_4096);

		dT = D2 - (uint32_t)C[5] * pow(2, 8);
		TEMP = (2000 + (dT * (int64_t)C[5] / pow(2, 23)));

		OFF = (int64_t)C[2] * pow(2, 16) + (dT*C[4]) / pow(2, 7);
		SENS = (int32_t)C[1] * pow(2, 15) + dT*C[3] / pow(2, 8);

		/*
		SECOND ORDER TEMPARATURE COMPENSATION
		*/
		if (TEMP < 2000) // if temperature lower than 20 Celsius 
		{
			int32_t T1 = 0;
			int64_t OFF1 = 0;
			int64_t SENS1 = 0;
			T1 = pow((double)dT, 2) / 2147483648LL;
			OFF1 = 5 * pow(((double)TEMP - 2000), 2) / 2;
			SENS1 = 5 * pow(((double)TEMP - 2000), 2) / 4;
			if (TEMP < -1500) // if temperature lower than -15 Celsius 
			{
				OFF1 = OFF1 + 7 * pow(((double)TEMP + 1500), 2);
				SENS1 = SENS1 + 11 * pow(((double)TEMP + 1500), 2) / 2;
			}

			TEMP -= T1;
			OFF -= OFF1;
			SENS -= SENS1;
		}
		P = ((((int64_t)D1*SENS) / pow(2, 21) - OFF) / pow(2, 15));

		Temparature = (double)TEMP / (double)100;
		Pressure = (double)P / (double)100;

		if (prevSampled_time == 0)
		{
			fltd_Temparature = Temparature;
			fltd_Pressure = Pressure;
		}

		fltd_Temparature = alpha * fltd_Temparature + (1 - alpha) * Temparature;
		fltd_Pressure = beta * fltd_Pressure + (1 - beta) * Pressure;

		Altitude = 44330.0f * (1.0f - pow((double)fltd_Pressure / 
			(double)SEA_LEVEL_PRESSURE,0.1902949f));
		if (prevSampled_time == 0)
		{
			pre_Altitude = Altitude;
		}
		roc = (int)(100000 * (Altitude - pre_Altitude) / Sampling_time);
		roc=roc/(2.54 * 12.0*60);
		if (prevSampled_time == 0)
		{
			fltd_roc = roc;
		}
		fltd_roc = gamma * fltd_roc + (1 - gamma) * roc;
		pre_Altitude = Altitude;
		/////////////////////////////////////////////////////////////////////////////

		//read MPU6050 sensor
		readMPU();
		//Get Degree Measurements for angles pitch and roll
		rollDeg=rollRad * 180/PI;
   		pitchDeg=pitchRad *180/PI;
   		//print Data
		//PrintData();
		float fAlt=(Altitude /.3048);
		float fPres = fltd_Pressure /33.8639;
		outF = fopen("DataFiles/Nav.txt","w");
		fprintf(outF,"%d %.2f %d %d",(int) fAlt,fPres,(int) pitchDeg,(int) rollDeg);
		fclose(outF);
		//Read at 10HZ (10 times per second)
		delay(100);
  return 0;
}
/*
    All contents of this file were written by Brandon Mord 
    bdrmord001@gmail.com
    
    Original owner of git code Bmord01
*/
