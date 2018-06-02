#include <string>
#include <ctime>

class Traffic {

    public:
        Traffic(std::string tailNum, float lat, float lon, int alt, int trk, int spd);
        ~Traffic();

        int Compare(const Traffic& d);
        int operator==(const Traffic& d);
        bool timeout();
        std::string toString();

        std::string getTail();
        float getLatitude();
        float getLongitude();
        int getAltitude();
        int getTrack();
        int getSpeed();

        void setTail(std::string);
        void setLat(float);
        void setLong(float);
        void setAlt(int);
        void setTrack(int);
        void setSpeed(int);
        

    private:
        std::string tail;
        float latitude;
        float longitude;
        int altitude;
        int track;
        int speed;
        time_t lastSeen;
        std::tm lastTime;
};