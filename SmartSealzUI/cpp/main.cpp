#include "easywsclient.hpp"
//#include "easywsclient.cpp" 
#include "json/json-forwards.h"
#include "json/json.h"
//#include "jsoncpp.cpp"
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string>
#include <list>
#include "traffic.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <thread>
#include <unistd.h>

//Test String for debugging
std::string TRAFFICTEST = "{\"Icao_addr\":2837120,\"OnGround\":false,\"Lat\":42.19293,\"Lng\":-83.92148,\"Position_valid\":true,\"Alt\":3400,\"Track\":9,\"Speed\":92,\"Speed_valid\":true,\"Vvel\":0,\"Tail\":\"N123475\",\"Last_seen\":\"2015-12-22T21:29:22.241048727Z\",\"Last_source\":2}";

//List structure to hold the aircraft information
std::list<Traffic> targets;

//Websocket lib and pointer setup
using easywsclient::WebSocket;
static WebSocket::pointer stratuxTraffic = NULL;

// TODO: Functionality to reconnect to stratux if connection is lost...
// TODO: Check file status to safely write and avoid seg faults...


//Iterates through the 'targets' list to find an aircraft with the given tail number
//Returns the iterator if found, else returns the end pointer which indicates nothing was found.
std::list<Traffic>::iterator findAircraft(std::string x){

    //Make sure the list isn't empty
    if (!targets.empty()){
        //get the front iterator from the list
        std::list<Traffic>::iterator it = targets.begin();

        for (it; it != targets.end(); it++){
            //Compares the strings of the tail numbers
            if((*it).getTail().compare(x) == 0){
                return it;
            }
        }
    }
    //returns the back iterator posistion, indicates nothing was found
    return targets.end();

}

/*Adds traffic objects to the list.
/ Design Rationale: Most recent traffic updates are pushed to the front, while least recent end up at the back
/ When an existing aircraft is updated, it searches for it, removes it, then pushes it to the front.
/ Only the 15 most recent targets are kept. Others will be popped off the back
/ We then iterate through the list to find aircraft that have timed out (no reports for more than 60 seconds)
/ If an aircraft times out, it is removed from the list
*/
void addTraffic(Traffic aircraft){

    //search for a an existing aircraft, and remove it
    std::list<Traffic>::iterator pos = findAircraft(aircraft.getTail());
    if (pos != targets.end()){
        targets.erase(pos);
    }

    //Pushes the target to the front of the list
    targets.push_front(aircraft);
    if (targets.size() > 15){
        targets.pop_back();  //More than 15 targets? remove the last one. only the 15 most recent targets are kept
    }

    //Print each target to know which ones are in the list.
    //printf("Target List Information\n");    

    //check for timed out Aircraft and remove them
    //reset iterator to front
    //*pos is used to access the value at the iterator posistion.
    pos = targets.begin();
    for (pos; pos != targets.end(); pos++){

        //printf("Target: %s\n", (*pos).getTail().c_str());
        
        if ((*pos).timeout()){
            printf("\nTIMED OUT! Removing...\n");
            pos = targets.erase(pos); //erase returns the iterator posisiton of the next item after the erase. This is important to prevent bad iterator references. 
        }
    }

}


//Prints aircraft infromation to 'traffic.txt' file
//Calls Traffics' toString method which is customized to follow the format given in the ADSB Format.txt file
//Needs to check that the file opened correctly (Make sure the main SmartSealz program isn't reading from it at the same time)
//Closes the file when finished
int count=0;
void exportData(){
    //Outstream object
    std::ofstream trafficFile;
    std::ofstream otherFile;
    //open the file in 'trunc' mode to create a new file regardless of current contents
    trafficFile.open("traffic.txt", std::ios::trunc);
    
    if (trafficFile.is_open()){
        for (Traffic x: targets){
            //toString returns a std::string, must be converted to a c_str() to print correctly
            trafficFile << x.toString().c_str();
        }
    }
    else {
        //printf("Could not open file for printing...");
    }

    //close the file when done.
    trafficFile.close();
    usleep(1075);
    count++;
    
}

//Parses the JSON string that is passed from the websocket
//Then adds the parsed object to the target list
void parseTraffic(const std::string & json, Json::Reader & reader){
    //JSON object root initlizied. 
    Json::Value root;

    if (reader.parse(json, root, false)){
        //Parses from the JSON ROOT to find all the given ID's and Values, the get() method requires a default value in case it cannot find the ID, this is to prevent obvious errors. 
        Traffic tmp = Traffic(root.get("Tail", "NO TAIL").asString(), root.get("Lat", -9999.0).asFloat(), root.get("Lng", -9999.0).asFloat(), root.get("Alt", -1).asInt(), root.get("Track", -1).asInt(), root.get("Speed", -1).asInt());
        //printf("\nNew Aircraft\n Tail: %s\n Lat: %f\n Long: %f\n", tmp.getTail().c_str(), tmp.getLatitude(), tmp.getLongitude());
        //add the object to the list
        addTraffic(tmp);
    }
    else{
        printf("Error while parsing");
    }

}

//Handler function that gets called when the websocket receives a message
//Initilized the JSON reader, and closes the websocket if a certain message is recieved
void handle_traffic_message(const std::string & message)
{
    Json::Reader jRead = Json::Reader();

    //printf("\n>>Message Recieved\n");
    parseTraffic(message, jRead);
    if (message == "end") { stratuxTraffic->close(); }
    
}


// Main 
// Sets up the websocket and runs the main while loop that polls the websocket and calls the appropriate handler function for the messages
// Theoretically can be run on a windows machine.. THIS HAS NOT BEEN TESTED
// This program has only been tested on a linux machine.
int main()
{
    FILE *outF;
#ifdef _WIN32
    INT rc;
    WSADATA wsaData;

    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc) {
        printf("WSAStartup Failed.\n");
        return 1;
    }
#endif

    stratuxTraffic = WebSocket::from_url("ws://192.168.10.1/traffic"); //getSituation
    if(stratuxTraffic==NULL){
        printf("Error in Main.cpp");
        outF=fopen("LoggingFile.txt","a");
        fprintf(outF,"Error in main.cpp");
        return 1;
    }
    assert(stratuxTraffic);

    while (stratuxTraffic->getReadyState() != WebSocket::CLOSED) {
      //Websocket functions
      stratuxTraffic->poll();
      stratuxTraffic->dispatch(handle_traffic_message);

      exportData(); //writes targets to file

    }
    delete stratuxTraffic;
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

/*
int main(){
    printf("TESTING JSON PARSING\n");

    printf("TESTING TRAFFIC\n");
    handle_message(TRAFFICTEST, "traffic");
    
    printf("TESTING GPS\n");
    handle_message(GPSTEST, "gps");
}
*/
