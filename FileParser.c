#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <math.h>
int main(int argc, char *argv){
	// replace 682 with file lines / 41
	char inTxt[50];
	int count =0;
	
	
	
	FILE * inptFile;
	printf("Enter File to Open: ");
	scanf("%s",&inTxt);
	inptFile = fopen(inTxt,"r");
	
	char *line = NULL;
	size_t len=0;
	ssize_t read;
	while((read = getline(&line,&len,inptFile))!=-1){
		count++; //count lines in traffic file
	}
	fclose(inptFile);
	inptFile=fopen(inTxt,"r");
	int size = count / 41;
	float user[size];
	float nice[size];
	float system[size];
	float iowait[size];
	float steal[size];
	float idle[size];
	float totalMem[size];
	float usedMem[size];
	float freeMem[size];
	float sharedMem[size];
	float buff[size];
	float cachedMem[size];
	float usedCMem[size];
	float freecMem[size];
	float totalSMem[size];
	float usedSMem[size];
	float freeSMem[size];
	
	int i,j;
	int bad=0;
	bool badD = false;
	char store[75];
	char *token;
	char *token2;
	int tn=0;
	//Read Trough Files and add items to indexes of array index - bad data indexes
	for(j=0;j<size;j++){
		for(i=0;i<41;i++){
			token = strtok(fgets(store,20,inptFile),"\n");
			printf("%s %d\n",token,tn++);
			if(((i%41)==14)&& !badD){
				if(atof(token)<2 || atof(token)> 75){
					badD=true;
					bad++;
					continue;
				}
				user[j-bad]=atof(token);
			}
			if(((i%41)==15)&& !badD){
				nice[j-bad]=atof(token);
			}
			if(((i%41)==16)&& !badD){
				system[j-bad]=atof(token);
			}
			if(((i%41)==17)&& !badD){
				iowait[j-bad]=atof(token);
			}
			if(((i%41)==18)&& !badD){
				steal[j-bad]=atof(token);
			}
			if(((i%41)==19)&& !badD){
				idle[j-bad]=atof(token);
			}
			if(((i%41)==27)&& !badD){
				totalMem[j-bad]=atof(token);
			}
			if(((i%41)==28)&& !badD){
				usedMem[j-bad]=atof(token);
			}
			if(((i%41)==29)&& !badD){
				freeMem[j-bad]=atof(token);
			}
			if(((i%41)==30)&& !badD){
				sharedMem[j-bad]=atof(token);
			}
			if(((i%41)==31)&& !badD){
				buff[j-bad]=atof(token);
			}
			if(((i%41)==32)&& !badD){
				cachedMem[j-bad]=atof(token);
			}
			if(((i%41)==35)&& !badD){
				usedCMem[j-bad]=atof(token);
			}
			if(((i%41)==36)&& !badD){
				freecMem[j-bad]=atof(token);
			}
			if(((i%41)==38)&& !badD){
				totalSMem[j-bad]=atof(token);
			}
			if(((i%41)==39)&& !badD){
				usedSMem[j-bad]=atof(token);
			}
			if(((i%41)==40)&& !badD){
				freeSMem[j-bad]=atof(token);
			}
		}
		badD=false;
	}
	/*float user[size];
	float nice[size];
	float system[size];
	float iowait[size];
	float steal[size];
	float idle[size];
	float totalMem[size];
	float usedMem[size];
	float freeMem[size];
	float sharedMem[size];
	float buff[size];
	float cachedMem[size];
	float usedCMem[size];
	float freecMem[size];
	float totalSMem[size];
	float usedSMem[size];
	float freeSMem[size];*/
	float maxU=0,minU=0,aveU=0,spreadU=0;
	float maxN=0,minN=0,aveN=0,spreadN=0;
	float maxS=0,minS=0,aveS=0,spreadS=0;
	float maxI=0,minI=0,aveI=0,spreadI=0;
	float maxST=0,minST=0,aveST=0,spreadST=0;
	float maxID=0,minID=0,aveID=0,spreadID=0;
	float maxT=0,minT=0,aveT=0,spreadT=0;
	float maxUM=0,minUM=0,aveUM=0,spreadUM=0;
	float maxF=0,minF=0,aveF=0,spreadF=0;
	float maxSM=0,minSM=0,aveSM=0,spreadSM=0;
	float maxB=0,minB=0,aveB=0,spreadB=0;
	float maxCM=0,minCM=0,aveCM=0,spreadCM=0;
	float maxUCM=0,minUCM=0,aveUCM=0,spreadUCM=0;
	float maxFC=0,minFC=0,aveFC=0,spreadFC=0;
	float maxTSM=0,minTSM=0,aveTSM=0,spreadTSM=0;
	float maxUSM=0,minUSM=0,aveUSM=0,spreadUSM=0;
	float maxFSM=0,minFSM=0,aveFSM=0,spreadFSM=0;
	//File Parsed Test print statement
	for(i=0;i<(size-bad);i++){
		aveU+=user[i];aveN+=nice[i];aveS+=system[i];aveI+=iowait[i];aveST+=steal[i];
		aveID+=idle[i];aveT+=totalMem[i];aveUM+=usedMem[i];aveF+=freeMem[i];
		aveSM+=sharedMem[i];aveB+=buff[i];aveCM+=cachedMem[i];aveUCM+=usedCMem[i];
		aveFC+=freecMem[i];aveTSM+=totalSMem[i];aveUSM+=usedSMem[i];aveFSM+=freeSMem[i];
			
		if(i==0){
			maxU=user[i];maxN=nice[i];maxS=system[i];maxI=iowait[i];maxST=steal[i];
			maxID=idle[i];maxT=totalMem[i];maxUM=usedMem[i];maxF=freeMem[i];
			maxSM=sharedMem[i];maxB=buff[i];maxCM=cachedMem[i];maxUCM=usedCMem[i];
			maxFC=freecMem[i];maxTSM=totalSMem[i];maxUSM=usedSMem[i];maxFSM=freeSMem[i];
			
			minU=user[i];minN=nice[i];minS=system[i];minI=iowait[i];minST=steal[i];
			minID=idle[i];minT=totalMem[i];minUM=usedMem[i];minF=freeMem[i];
			minSM=sharedMem[i];minB=buff[i];minCM=cachedMem[i];minUCM=usedCMem[i];
			minFC=freecMem[i];minTSM=totalSMem[i];minUSM=usedSMem[i];minFSM=freeSMem[i];
			
		}
		if(maxU<user[i]){maxU=user[i];}
		if(minU>user[i]){minU=user[i];}
		if(maxN<nice[i]){maxN=nice[i];}
		if(minN>nice[i]){minN=nice[i];}
		if(maxS<system[i]){maxS=system[i];}
		if(minS>system[i]){minS=system[i];}
		if(maxI<iowait[i]){maxI=iowait[i];}
		if(minI>iowait[i]){minI=iowait[i];}
		if(maxST<steal[i]){maxST=steal[i];}
		if(minST>steal[i]){minST=steal[i];}
		if(maxID<idle[i]){maxID=idle[i];}
		if(minID>idle[i]){minID=idle[i];}
		if(maxT<totalMem[i]){maxT=totalMem[i];}
		if(minT>totalMem[i]){minT=totalMem[i];}
		if(maxUM<usedMem[i]){maxUM=usedMem[i];}
		if(minUM>usedMem[i]){minUM=usedMem[i];}
		if(maxF<freeMem[i]){maxF=freeMem[i];}
		if(minF>freeMem[i]){minF=freeMem[i];}
		if(maxSM<sharedMem[i]){maxSM=sharedMem[i];}
		if(minSM>sharedMem[i]){minSM=sharedMem[i];}
		if(maxB<buff[i]){maxB=buff[i];}
		if(minB>buff[i]){minB=buff[i];}
		if(maxCM<cachedMem[i]){maxCM=cachedMem[i];}
		if(minCM>cachedMem[i]){minCM=cachedMem[i];}
		if(maxUCM<usedCMem[i]){maxUCM=usedCMem[i];}
		if(minUCM>usedCMem[i]){minUCM=usedCMem[i];}
		if(maxFC<freecMem[i]){maxFC=freecMem[i];}
		if(minFC>freecMem[i]){minFC=freecMem[i];}
		if(maxTSM<totalSMem[i]){maxTSM=totalSMem[i];}
		if(minTSM>totalSMem[i]){minTSM=totalSMem[i];}
		if(maxUSM<usedSMem[i]){maxUSM=usedSMem[i];}
		if(minUSM>usedSMem[i]){minUSM=usedSMem[i];}
		if(maxFSM<freeSMem[i]){maxFSM=freeSMem[i];}
		if(minFSM>freeSMem[i]){minFSM=freeSMem[i];}
	}
	spreadU=maxU-minU;
	spreadN=maxN-minN;
	spreadS=maxS-minS;
	spreadI=maxI-minI;
	spreadST=maxST-minST;
	spreadID=maxID-minID;
	spreadT=maxT-minT;
	spreadUM=maxUM-minUM;
	spreadF=maxF-minF;
	spreadSM=maxSM-minSM;
	spreadB=maxB-minB;
	spreadCM=maxCM-minCM;
	spreadUCM=maxUCM-minUCM;
	spreadFC=maxFC-minFC;
	spreadTSM=maxTSM-minTSM;
	spreadUSM=maxUSM-minUSM;
	spreadFSM=maxFSM-minFSM;
	
	aveU = aveU / (size-bad);
	aveN = aveN / (size-bad);
	aveS = aveS / (size-bad);
	aveI = aveI / (size-bad);
	aveST = aveST / (size-bad);
	aveID = aveID / (size-bad);
	aveT = aveT / (size-bad);
	aveUM = aveUM / (size-bad);
	aveF = aveF / (size-bad);
	aveSM = aveSM / (size-bad);
	aveB = aveB / (size-bad);
	aveCM = aveCM / (size-bad);
	aveUCM = aveUCM / (size-bad);
	aveFC = aveFC / (size-bad);
	aveTSM = aveTSM / (size-bad);
	aveUSM = aveUSM / (size-bad);
	aveFSM = aveFSM / (size-bad);
	
	printf("\n\n%d %.2lf %.2lf\n",bad,aveU,spreadU);
	fclose(inptFile);
	return 0;
}
