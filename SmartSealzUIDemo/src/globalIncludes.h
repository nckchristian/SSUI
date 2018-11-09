/*
    All contents of this file were written by Brandon Mord 
    bdrmord001@gmail.com
    
    Original owner of git code Bmord01
*/
#include <gtk/gtk.h>
#include <glib.h>
#include  <stdio.h>
#include  <math.h>
#include <stdio.h>
#include  <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#define PI 3.14159265359
/******************************************************************************/
/**************************** Variables ***************************************/
//Full Version Variables
//float HighSensePressureEB=50.0,HighSensePitchEB=15.0,HighSenseRollEB=20.0,HighSenseAltitudeEB=50.0, HighSenseGPSTrack = 5.0,HighSenseGPSGroundS=50.0;
//float LowSensePressureEB=150.0,LowSensePitchEB=15.0,LowSenseRollEB=55.0,LowSenseAltitudeEB=150.0,LowSenseGPSTrack = 15.0,LowSenseGPSGroundS=150.0;

//Demo Variables:
float HighSensePressureEB=2.0,HighSensePitchEB=10.0,HighSenseRollEB=10.0,HighSenseAltitudeEB=2.0,HighSenseGPSTrack = 5.0,HighSenseGPSGroundS=2.0;
float LowSensePressureEB=5.0,LowSensePitchEB=30.0,LowSenseRollEB=30.0,LowSenseAltitudeEB=4.0,LowSenseGPSTrack = 15.0,LowSenseGPSGroundS=5.0;

static gboolean continue_timer = FALSE;
static gboolean start_timer=FALSE;
//static int times_cycled = 0;

double GPSLat, GPSLong, GPSCourse, GPSGroundSpeed, ADSBPressure, ADSBPitch, ADSBRoll, ADSBGyroHeading, ADSBMagHeading;
double holdCourse,holdGS;
double Range=0.0;
double dist=0.0;

int testc=0;
int prevTrafficC;
int zeroCount=0;
int altim;
int inAlt,inPitch,inRoll,HR,prevHR=0;
int holdAlt,holdPitch,holdRoll;
int TrafficCount=0;

bool Started = FALSE;
bool oneS, fiveS,tenS;
bool activeS;
bool bAlt=false,bPitch=false,bRoll=false,bGS=false,bMagHead=false,bGPST=false;
bool bAltA=false,bPitchA=false,bRollA=false,bGSA=false,bMagHeadA=false,bGPSTA=false;
bool bAltC=false,bPitchC=false,bRollC=false,bGSC=false,bMagHeadC=false,bGPSTC=false;
bool display = false;
bool threadRunning=false;
bool Background = false;
bool skip=false;
pthread_t tid[5];
pid_t pid;

float inPres;
float holdPres;

char destAlt[10],destHoldAlt[10];
char destPres[10],destHoldPres[10];
char destPitch[10],destHoldPitch[10];
char destRoll[10],destHoldRoll[10];
char destGroundS[10],destHoldGroundS[10];
char destCourse[10],destHoldCourse[10];
char destHR[5];
char destAltim[10];
char destTraffic[30];

/*************************************************************************************************************/
/****************************************GTK Pointer Definition **********************************************/
GtkButton *btnStart,*btnEnd,*btnSet,*btnCalHap;

GtkLabel *lblAltitude,*lblPitch,*lblRoll, *lblGS, *lblGPST,*lblHR, *lblCurr,*lblTraffic,*warning;

GtkSwitch *Sense1,*Sense2,*Sense3,*Sense4,*Sense5,*Sense6;

GtkToggleButton *tbAlt,*tbPitch,*tbRoll,*tbGS,*tbGPST,*tbHR,*tbActiveTraffic, *tbOneM, *tbFiveM, *tbTenM;

GtkSpinButton *spAltim;
/*****************************************************************************/
void *upHaptic(void *vargp);
void *upTraffic(void *vargp);
double CalcDist(double inLat,double inLong);

FILE *inGPS;

