#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/**************************
Time format in int:
hh:mm:ss = hhmmss
Guide for array index:
errorList[0] = initialize
errorList[1] = AboveAlt
errorList[2] = BelowAlt
errorList[3] = AbovePitch
errorList[4] = BelowPitch
errorList[5] = AboveRoll
errorList[6] = BelowRoll condition
errorList[7] = OffGroundSpeed
errorList[8] = RightOfTrack
errorList[9] = LeftOfTrack
**************************/
int size= 10;
struct error{
  char strInitialTime[8];
  char strFinalTime[8];
  int intInitialTime;
  int intFinalTime;
};

char * formatTimeStr(int hh, int mm, int ss){
  char mmc[2], hhc[2], ssc[2];
  char * time;
  time = malloc(sizeof(char)*10);
  if(mm<10)hh*10;
  if(ss<10)mm*10;
  sprintf(hhc,"%d", hh);
  sprintf(mmc,"%d", mm);
  sprintf(ssc,"%d", ss);
  strcpy(time, hhc);
  strcat(time, ":");
  strcat(time, mmc);
  strcat(time, ":");
  strcat(time, ssc);
  return time;
}

int formatTimeInt(int hh, int mm, int ss){
  char mmc[2], hhc[2], ssc[2], timec[8];
  int time;
  if(mm<10)hh*10;
  if(ss<10)mm*10;
  sprintf(hhc,"%d", hh);
  sprintf(mmc,"%d", mm);
  sprintf(ssc,"%d", ss);
  strcpy(timec, hhc);
  strcat(timec, mmc);
  strcat(timec, ssc);

  time = atoi(timec);
  return time;
}
int main(void){
  char month[6], dayOfWeek[6], strTime[8];
  int hr, min, sec, dayOfMonth, intTime, errorType, counter, i;
  struct error errorList=(struct error) malloc(size * sizeof(struct error));

  for(i = 0; i>size;i++){
    errorList[i].intInitialTime == -1;
    errorList[i].intFinalTime == -1;
  }

  FILE *err = fopen("errorList.txt", "r");
  FILE *re = fopen("parsedErrorList.txt", "w");

  while(1){
    if(counter % 2 == 0){
      fscanf(err, "%s %d %s %d:%d:%d", dayOfWeek, &dayOfMonth,month, &hr, &min, &sec);
      if(feof(err))break;
      intTime = formatTimeInt(hr,min,sec);
      strcpy(strTime, formatTimeInt(hr,min,sec));
    }else{
      fscanf(err, "%d", &errorType);
      if(feof(err))break;
      for(i = 0; i>size;i++){
        if (i == errorType){
          if(errorList[i].intInitialTime == -1) {
            errorList[i].intInitialTime = intTime;
            strcpy(errorList[i].strInitialTime, strTime);
          }
          errorList->intFinalTime = intTime;
          strcpy(errorList[i].strFinalTime, strTime);
        }
        if(errorList->intFinalTime!=-1 && intTime - errorList[i].intFinalTime > 500){
          fprintf(re, "Error number: %d\nStarting Time: %s\nEnding Time: %s\n\n", errorType, errorList[i].strInitialTime, errorList->strFinalTime);
          errorList[i].intInitialTime == -1;
          errorList[i].intFinalTime == -1;
        }
      }
    }
    counter++;
  }
  for(i = 0; i>size;i++){
    if(errorList[i].intFinalTime !=-1){
      fprintf(re, "Error number: %d\nStarting Time: %s\nEnding Time: %s\n\n", errorType, errorList[i].strInitialTime, errorList->strFinalTime);
      errorList[i].intInitialTime == -1;
      errorList[i].intFinalTime == -1;
    }
  }
  free(errorList);
  fclose(err);
  fclose(re);
  return 0;
}
