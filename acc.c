/*Description: The following code initializes the MMA8452Q 3 axis accelerometer  by writing specific 
values to the control registers used and also reads the acceleration values along the X, Y, Z axes
The X, Y and Z values are 12 bit values

Authors: Smitha Sunil Kamat, Jay Khandhar
*/

/*Preprocessor directives and user defined header files*/
#include <stdio.h>
#include <float.h>
#include "i2c_rw.h"
#include "acc.h"

/*Declaration of global variables*/
unsigned char temp;
float AccG_X, AccG_Y, AccG_Z; /*Variables to store the g values of acceleration*/
unsigned int conv_raw_xconcat,conv_raw_yconcat,conv_raw_zconcat; /*Variables to store the corrected X, Y and Z acceleration values*/


/*Initialization sequence of the Accelerometer*/
void initAcc(void)
{	
	unsigned char fsr;
	
	/*Read the device identification register of the accelerometer*/
	temp = read_reg(acc_slvAddr,acc_devId);
	if(temp == 0x2A)
		printf("The Accelerometer is on I2C bus - 1\n");
	else
		printf("ERROR !!! - Accelerometer not found on I2C bus - 1\n");

	/*Read CTRL_REG1 and clear its LSB to drive the accelerometer to STANDBY mode*/
	temp = read_reg(acc_slvAddr,A_CTRL_REG1);
	temp = temp & ~(0x01);
	
	/*Write into CTRL_REG1 to drive it to stand by mode*/
	/*The accelerometer should be driven to STANDBY mode before making changes to any other registers*/
	write_reg(acc_slvAddr,A_CTRL_REG1,temp);
	
	/*Setting up the GSCALE value to measure acceleration in the -2g to +2g range*/	
	fsr = GSCALE;
	if(fsr > 8) fsr = 8;
	fsr = fsr >> 2;
	write_reg(acc_slvAddr,XYZ_DATA_CFG,fsr);

	/*Bring back the Accelerometer to ACTIVE mode*/
	temp = read_reg(acc_slvAddr,A_CTRL_REG1);
	temp = temp | 0x01;
	write_reg(acc_slvAddr,A_CTRL_REG1,temp);
	
}

/*Read the X,Y,Z axes values of the Accelerometer*/
void readAcc_XYZ(void)
{
	unsigned int xlow,xhigh,ylow,yhigh,zlow,zhigh;
	unsigned int xconcat, yconcat, zconcat;
	unsigned int raw_xconcat, raw_yconcat, raw_zconcat;

	/*Read the MSB and LSB registers of the accelerometer*/
	xhigh = read_reg(acc_slvAddr,OUT_X_H);
	xlow = read_reg(acc_slvAddr,OUT_X_L);

	yhigh = read_reg(acc_slvAddr,OUT_Y_H);
	ylow = read_reg(acc_slvAddr,OUT_Y_L);

	zhigh = read_reg(acc_slvAddr,OUT_Z_H);
	zlow = read_reg(acc_slvAddr,OUT_Z_L);


	/*Concatenate the values obtained from the LSB and MSB registers for X, Y and Z acceleration values*/

	xconcat = (xhigh << 8) + xlow;
	raw_xconcat = xconcat >> 4;

	yconcat = (yhigh << 8) + ylow;
	raw_yconcat = yconcat >> 4;

	zconcat = (zhigh << 8) + zlow;
	raw_zconcat = zconcat >> 4;

	/*Taking care of 2's complement*/
	/*The output values stored in the X, Y and Z registers is in the 2's complement form*/
	/*The following algorithm helps differentiate between negative and positive acceleration values*/
	/*The acceleration values range from 0 to 65535.*/
	/*The acceleration values from 0 to 32727 are positive and others are negative*/

	/*Since we are considering only horizontal movement, we consider only the Y axis values of the accelerometer*/


	if(raw_xconcat > 0x7FF)
	{
		conv_raw_xconcat = -(raw_xconcat - 0x7FF) + 1;
		AccG_X = -((float)(conv_raw_xconcat * GSCALE)/(float)0x7FF);
	}
	else
	{
		conv_raw_xconcat = raw_xconcat;
		AccG_X =((float)(conv_raw_xconcat * GSCALE)/(float)0x7FF);
	}

	if(raw_yconcat > 0x7FF)
	{
		conv_raw_yconcat = -(raw_yconcat - 0x7FF) + 1;
	}
	else
	{
		conv_raw_yconcat = raw_yconcat;
	}

	if(raw_zconcat > 0x7FF)
	{
		conv_raw_zconcat = -(raw_zconcat - 0x7FF) + 1;
	}
	else
	{
		conv_raw_zconcat = raw_zconcat;
	}
}

