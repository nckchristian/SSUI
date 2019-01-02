unsigned int PROM_read(int DA, char PROM_CMD);
long CONV_read(int DA, char CONV_CMD);
int read_word_2c(int addr);
float comp_filter(float prevAngle, float accelAngle, float gyroRate);
double dist(double a, double b);
double get_y_rotation(double x, double y, double z);
double get_x_rotation(double x, double y, double z);
float noTiltCompensate(float YAxis, float XAxis);
float correctAngle(float heading);
float tiltCompensate(float YAxis, float XAxis, float ZAxis, float nPitch, float nRoll); //Unused
void PrintData();
void readMPU();

