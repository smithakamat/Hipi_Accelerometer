#include <stdio.h>
#include "i2c_rw.h"
#include "acc.h"

unsigned char temp;

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

	/*Read CTRL_REG1 and clear its LSB*/
	temp = read_reg(acc_slvAddr,A_CTRL_REG1);
	temp = temp & ~(0x01);
	
	/*Write into CTRL_REG1 to drive it to stand by mode*/
	/*The accelerometer should be driven to STANDBY mode before making changes to any other registers*/
	write_reg(acc_slvAddr,A_CTRL_REG1,temp);
	
	/*Settimg up the GSCALE value*/	
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

	xhigh = read_reg(acc_slvAddr,OUT_X_H);
	xlow = read_reg(acc_slvAddr,OUT_X_L);

	yhigh = read_reg(acc_slvAddr,OUT_Y_H);
	ylow = read_reg(acc_slvAddr,OUT_Y_L);

	zhigh = read_reg(acc_slvAddr,OUT_Z_H);
	zlow = read_reg(acc_slvAddr,OUT_Z_L);

	xconcat = (xhigh << 8) + xlow;
	xconcat = xconcat >> 4;

	yconcat = (yhigh << 8) + ylow;
	yconcat = yconcat >> 4;

	zconcat = (zhigh << 8) + zlow;
	zconcat = zconcat >> 4;

	/*Taking care of 2's complement*/
	if(xconcat > 0x7FF)
		xconcat = -(xconcat - 0x7FF) + 1;
	else
		xconcat = xconcat;

	if(yconcat > 0x7FF)
		yconcat = -(yconcat - 0x7FF) + 1;
	else
		yconcat = yconcat;

	if(zconcat > 0x7FF)
		zconcat = -(zconcat - 0x7FF) + 1;
	else
		zconcat = zconcat;

	printf("Acc_X : Acc_Y : Acc_Z : %d, %d, %d\n",xconcat, yconcat, zconcat);
}

