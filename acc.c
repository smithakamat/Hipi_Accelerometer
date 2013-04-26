#include <stdio.h>
#include <float.h>
#include "i2c_rw.h"
#include "acc.h"

unsigned char temp;
float AccG_X, AccG_Y, AccG_Z;
unsigned int conv_raw_xconcat,conv_raw_yconcat,conv_raw_zconcat;
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
	unsigned int raw_xconcat, raw_yconcat, raw_zconcat;
	//unsigned int conv_raw_xconcat, conv_raw_yconcat, conv_raw_zconcat;
	//float AccG_X, AccG_Y, AccG_Z;

	xhigh = read_reg(acc_slvAddr,OUT_X_H);
	xlow = read_reg(acc_slvAddr,OUT_X_L);

	yhigh = read_reg(acc_slvAddr,OUT_Y_H);
	ylow = read_reg(acc_slvAddr,OUT_Y_L);

	zhigh = read_reg(acc_slvAddr,OUT_Z_H);
	zlow = read_reg(acc_slvAddr,OUT_Z_L);

	xconcat = (xhigh << 8) + xlow;
	raw_xconcat = xconcat >> 4;

	yconcat = (yhigh << 8) + ylow;
	raw_yconcat = yconcat >> 4;

	zconcat = (zhigh << 8) + zlow;
	raw_zconcat = zconcat >> 4;

	/*Taking care of 2's complement*/
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
		//AccG_Y = -(float)(raw_yconcat * GSCALE)/0x7FF;
	}
	else
	{
		conv_raw_yconcat = raw_yconcat;
		//AccG_Y = (raw_yconcat * GSCALE)/0x7FF;
	}

	if(raw_zconcat > 0x7FF)
	{
		conv_raw_zconcat = -(raw_zconcat - 0x7FF) + 1;
		//AccG_Z = -(raw_zconcat * GSCALE)/0x7FF;
	}
	else
	{
		conv_raw_zconcat = raw_zconcat;
		//AccG_Z = (raw_zconcat * GSCALE)/0x7FF;
	}
	//printf("Raw_x : Raw_Y : Raw_Z : %d, %d, %d\n",raw_xconcat, raw_yconcat, raw_zconcat);
	//printf("Acc_X : Acc_Y : Acc_Z : %d, %d, %d\n",conv_raw_xconcat, conv_raw_yconcat, conv_raw_zconcat);
	//printf("AccG_X : AccG_Y : AccG_Z : %f, %f, %f\n", AccG_X, AccG_Y, AccG_Z);
//	printf("Converted Y value %d \n",conv_raw_yconcat);
//	printf("----------------------------------------------------\n");
}

