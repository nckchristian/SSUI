/*!
 * \brief SmartSealz Project
 * \details SmartSealz Development
 * \details Script Used For Installing All Required Packages
 * \author Brandon Mord
 * \date 2017-2019
 */


/*
    All contents of this file were written by Brandon Mord
    bdrmord001@gmail.com

    Original owner of git code Bmord01
*/
/** \defgroup install Install
 * @{
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @addtogroup System SystemCalls
 * System Calls Performed by Install Script
 * @{
 */

/** installGlade
 * @brief Install Glade Editing Tools on Linux Machine
 *
 */
void installGlade(){
    system("sudo apt-get install glade -y");
}
/** installGTKlib
 * @brief Install GTK Library Tools on Linux Machine
 */
void installGTKlib(){
    system("sudo apt-get install libgtk-3-dev -y");
}
/** installI2Clib
 * @brief Install I2C Library Tools on Linux Machine
 */
void installI2Clib(){
    system("sudo apt-get install libi2c-dev -y");
}
/** installSPICore
 * @brief Install SPI port Tools on Linux Machine
 */
void installSPICore(){
    system("sudo apt-get install at-spi2-core -y");
}
/** installPerl
 * @brief Install Perl Compilation Tools on Linux Machine
 */
void installPerl(){
    system("sudo perl -MCPAN -e'install \"LWP::Simple\"\'");
}
/** installADAFRUIT
 * @brief Install Adafruit Library Tools on Linux Machine
 */
void installADAFRUIT(){
    system("sudo pip3 install adafruit-ads1x15");
}
/** upgrade
 * @brief Run System Upgrade on Linux Machine
 */
void upgrade(){
    system("sudo apt-get upgrade");
}
/** update
 * @brief Run System Update on Linux Machine
 */
void update(){
    system("sudo apt-get update");
}
/** @} */ /* end of System */
int main(){

	installGlade();
	installGTKlib();
	installI2Clib();
	installSPICore();
	installPerl();
	installADAFRUIT();
	upgrade();
	update();
	return 0;
}
/** @} */ /*end of install */
