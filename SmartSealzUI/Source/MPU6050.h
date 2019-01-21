/** \ingroup MPU6050 MPU6050 Funcions
 * @{
 */
 
/** @addtogroup MPUFunctions Functions
 * Functions used by the PU6050 Source
 * @{
 */
 
/** Prom_read
 * 
 */
unsigned int PROM_read(int DA, char PROM_CMD);
/** CONV_read
 *
 */
long CONV_read(int DA, char CONV_CMD);
/** read_word
 *
 */
int read_word_2c(int addr);
/**comp_filter
 *
 */
float comp_filter(float prevAngle, float accelAngle, float gyroRate);
/** dist
 * Distance Formula
 */
double dist(double a, double b);
/** get_y_rotation
 * Returns Degree Rotation of X plane
 */
double get_y_rotation(double x, double y, double z);
/** get_x_rotation
 * Returns Degree Rotation of X plane
 */
double get_x_rotation(double x, double y, double z);
/** noTiltCompensate
 *
 */
float noTiltCompensate(float YAxis, float XAxis);
/** correctAngle
 * Normalize the heading Angle
 */
float correctAngle(float heading);
/** tiltCompensate
 * Unused Function
 */
float tiltCompensate(float YAxis, float XAxis, float ZAxis, float nPitch, float nRoll);
/** PrintData
 * Print the Data Out to the console if desired
 */
void PrintData();
/** readMPU
 * Read the data from the MPU6050 Chipset
 */
void readMPU();

/** @} */ /* end of MPUFunctions */
/** @} */ /* end of MPU6050 */

