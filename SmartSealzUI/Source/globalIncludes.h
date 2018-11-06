/*
    All contents of this file were written by Brandon Mord 
    bdrmord001@gmail.com
    
    Original owner of git code Bmord01
*/
/*This is the main driving program for the Navigation data from the GY-86 Chip*/
//Libraries Included
#include  <wiringPiI2C.h>
#include  <stdio.h>
#include  <math.h>
#define clear() printf("\033[H\033[J")
#define PI 3.141592
#define MPU6050_ADDRESS 0b1101000
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <time.h>

//Definition for chip ports
#define MS5611_ADDRESS 0x77

#define CONV_D1_256   0x40
#define CONV_D1_512   0x42
#define CONV_D1_1024  0x44
#define CONV_D1_2048  0x46
#define CONV_D1_4096  0x48
#define CONV_D2_256   0x50
#define CONV_D2_512   0x52
#define CONV_D2_1024  0x54
#define CONV_D2_2048  0x56
#define CONV_D2_4096  0x58
#define CMD_ADC_READ  0x00
#define CMD_PROM_READ 0xA0

#define OSR_256      1000 //us
#define OSR_512      2000 //us
#define OSR_1024     3000 //us
#define OSR_2048     5000 //us
#define OSR_4096     10000 //us

//Global Definitions
#define alpha 0.96
#define beta 0.96
#define gamma 0.96

/******************************************************************************/
// ================================================================
// ===                  Global Variables                        ===
// ================================================================
int fd;
int acclX, acclY, acclZ;
int gyroX, gyroY, gyroZ,temp;
double acclX_scaled, acclY_scaled, acclZ_scaled;
double gyroX_scaled, gyroY_scaled, gyroZ_scaled;
float dt=0.02;
int rX, rY, rZ;
float prevPitch = 0;
float prevRoll = 0;
float gyroPitch = 0.0;
float gyroRoll = 0.0;
float gyroYaw = 0.0;
float accelPitch = 0.0;
float accelRoll = 0.0;
float roll = 0.0;
float pitch = 0.0;
float yaw = 0.0;
float yawRad;
float rotX=0.0,rotY=0.0;
float pitchDeg=0.0,rollDeg=0.0,yawDeg=0.0;
double Temparature=0.0, fltd_Temparature=0.0;
double Pressure=0.0, fltd_Pressure=0.0;
float Altitude=0.0, pre_Altitude=0.0;
int roc=0, fltd_roc=0;
long curSampled_time = 0;
long prevSampled_time = 0;
float Sampling_time=0.0, prevSampling_time=0.0;
float rollRad=0.0,pitchRad=0.0;
int xMag=0,yMag=0,zMag=0;
struct timespec spec;
double SEA_LEVEL_PRESSURE;
////////////////////////////////////////////////////////////////////////////////
