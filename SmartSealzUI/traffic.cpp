#include <string>
#include "cpp/traffic.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <time.h>



Traffic::Traffic(std::string tailNum, float lat, float lon, int alt, int trk, int spd){
    tail = tailNum;
    latitude = lat;
    longitude = lon;
    altitude = alt;
    track = trk;
    speed = spd;
    time(&lastSeen);

}

Traffic::~Traffic(){

}

std::string Traffic::toString(){
    std::string tmp = getTail() + " " + std::to_string(getLatitude()) + " " + std::to_string(getLongitude()) + " " + std::to_string(getAltitude()) + " " + std::to_string(getTrack()) + " " + std::to_string(getSpeed()) + "\n";
    return tmp;
}

bool Traffic::timeout(){ //returns true if traffic timed out. 
    //get local time
    time_t now;
    time(&now);

    double diff;

    diff = difftime(now, lastSeen);
    if (diff > (double)60){
        printf("%f second(s) old", diff);
        return true;
    }
    
    return false;
}


int Traffic::Compare(const Traffic& d){
    if (tail.compare(d.tail) == 0){
        return 0;
    }
    else {
        return -1;
    }
}

int Traffic::operator==(const Traffic& d){
    if (tail.compare(d.tail) == 0){
        return 0;
    }
    else {
        return -1;
    }
}

std::string Traffic::getTail(){
    return tail;
}
float Traffic::getLatitude(){
    return latitude;
}
float Traffic::getLongitude(){
    return longitude;
}
int Traffic::getAltitude(){
    return altitude;
}
int Traffic::getTrack(){
    return track;
}
int Traffic::getSpeed(){
    return speed;
}


void Traffic::setTail(std::string tailNum){
    tail = tailNum;
}
void Traffic::setLat(float lat){
    latitude = lat;
}
void Traffic::setLong(float lon){
    longitude = lon;
}
void Traffic::setAlt(int alt){
    altitude = alt;
}
void Traffic::setTrack(int trk){
    track = trk;
}
void Traffic::setSpeed(int spd){
    speed = spd;
}
