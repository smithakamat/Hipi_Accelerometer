#include <stdio.h>
#include "gyro.h"
#include "i2c_rw.h"

unsigned char temp;

/*Initialization sequence of the gyroscope*/
void initGyro(void)
{
	/*Read the device identification register of the Gyroscope*/
	temp = read_reg(gyro_slvAddr,gyro_WHO_AM_I);
	printf("The dev ID if gyro is %d\n", temp);
	if(temp == 0xD4 )
		printf("The Gyroscope is on I2C bus - 1\n");
	else
		printf("ERROR !!! - Gyroscope  not found on I2C bus - 1\n");

	
	/*Write into CTRL_REG1 in order to enable x, y and z axes output of the gyro*/
	temp = 0x0F;
	write_reg(gyro_slvAddr,CTRL_REG1,temp);

	/*Disable the high pass filter of the gyro*/
	write_reg(gyro_slvAddr,CTRL_REG2,0x00);
	
	/*Enabling the DRDY pin of the gyro*/
	temp = 0x08;
	write_reg(gyro_slvAddr,CTRL_REG3,temp);

	/*Initializing CTRL_REG4*/
	write_reg(gyro_slvAddr,CTRL_REG4,0x00);	

	/*Initializing CTRL_REG5*/
	write_reg(gyro_slvAddr,CTRL_REG5,0x00);

	sleep(1);

	/*Bring the gyro out of power down mode*/
	temp = 0x00;
	write_reg(gyro_slvAddr,CTRL_REG1,temp);
}

/*Reading the X,Y,Z axes values of the gyroscope*/
void readGyro_XYZ(void)
{
	unsigned char xlow, xhigh, ylow, yhigh, zlow, zhigh;
	unsigned int xtotal, ytotal, ztotal;

	xlow = read_reg(gyro_slvAddr,OUT_X_Lo);
	xhigh = read_reg(gyro_slvAddr,OUT_X_Hi);
	
	ylow = read_reg(gyro_slvAddr,OUT_Y_Lo);
	yhigh = read_reg(gyro_slvAddr,OUT_Y_Hi);
	
	zlow = read_reg(gyro_slvAddr,OUT_Z_Lo);
	zhigh = read_reg(gyro_slvAddr,OUT_Z_Hi);

	xtotal = (xhigh << 8) + xlow;
	ytotal = (yhigh << 8) + ylow;
	ztotal = (zhigh << 8) + zlow;

	if(xtotal > 0x7FFF)
		xtotal = -(xtotal - 0x7FFF) + 1;
	else
		xtotal = xtotal;
	
	if(ytotal > 0x7FFF)
		ytotal = -(ytotal - 0x7FFF) + 1;
	else
		ytotal = ytotal;

	if(ztotal > 0x7FFF)
		ztotal = -(ztotal - 0x7FFF) + 1;
	else
		ztotal = ztotal;
	
	printf("Gyro_X : Gyro_Y : Gyro_Z : %d, %d, %d\n", xtotal, ytotal, ztotal);		
}
