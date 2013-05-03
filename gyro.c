/*Description: The following code is used to initialize the L3GD20 3 axis gyroscope. 
The angular velocity values are then read from the corressponding  X, Y and Z axis registers.

Authors: Smitha Sunil Kamat, Jay Khandhar*/


/*Preprocessor directives and user defined header files*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>   
#include "gyro.h"
#include "i2c_rw.h"

/*Declaration of global variables and structures*/

struct timespec gyro_time;

/*Variables that store the angle in degrees per second*/
float xtotal, ytotal, ztotal;

/*Variables that store the +/-  raw gyro digital values*/
unsigned int raw_x,raw_y,raw_z;
unsigned int avg_gyro_y;

/*Variable for raw teperature sensor values from the gyro*/
unsigned int raw_temp;



/*Initialization sequence of the gyroscope*/
void initGyro(void)
{
	unsigned char temp,dev_identify;

	/*Read the device identification register of the Gyroscope*/
	dev_identify = read_reg(0x6B,0x0F);
		printf("The Gyroscope is on I2C bus - 1\n");

	
	/*Write into CTRL_REG1 in order to enable x, y and z axes output of the gyro*/
	temp = 0x0F;
	write_reg(gyro_slvAddr,CTRL_REG1,temp);
	
	/*Disable the high pass filter mode selection of the gyro*/
	write_reg(gyro_slvAddr,CTRL_REG2,0x00);
	
	/*Enabling the DRDY pin of the gyro*/
	temp = 0x08;
	write_reg(gyro_slvAddr,CTRL_REG3,temp);
	
	/*Initializing CTRL_REG4 to select little endian data selection and to set the full scale selection to 250 dps*/
	write_reg(gyro_slvAddr,CTRL_REG4,0x00);	

	/*Initializing CTRL_REG5 to disable the high pass filter and FIFO*/
	write_reg(gyro_slvAddr,CTRL_REG5,0x00);

	sleep(1);
       

	/*Bring the gyro out of power down mode*/
	temp = 0x0F;
	write_reg(gyro_slvAddr,CTRL_REG1,temp);

        
	
}

/*Reading the X,Y,Z axes values of the gyroscope*/
void readGyro_XYZ(void)
{
	unsigned char xlow=0, xhigh = 0, ylow=0, yhigh=0, zlow=0, zhigh=0;

	float elapsedTime;

	/*The angular velocity values of the gyroscope along X, Y and Z axes are 16 bit values*/
	/*The values range from 0 to 65535. The positive angular velocity values are from 0 to 32767*/
	/*The values from 32767 to 65535 are negative*/


	xlow = read_reg(gyro_slvAddr,OUT_X_Lo);
	xhigh = read_reg(gyro_slvAddr,OUT_X_Hi);
	
	ylow = read_reg(gyro_slvAddr,OUT_Y_Lo);
	yhigh = read_reg(gyro_slvAddr,OUT_Y_Hi);
	
	zlow = read_reg(gyro_slvAddr,OUT_Z_Lo);
	zhigh = read_reg(gyro_slvAddr,OUT_Z_Hi);

	/*Obtain the timestamp when the X,Y, Z gyro values are read*/
	clock_gettime(CLOCK_MONOTONIC,&gyro_time);
	
	/*Concatenate the LSB and MSB values of the X, Y and Z register values of the gyroscope to obtain the 12 bi value*/
	raw_x = (xhigh << 8) + xlow;
	raw_y = (yhigh << 8) + ylow;
	raw_z = (zhigh << 8) + zlow;
	

	/*Since we are considering only the horizontal movement of the PCB, we are considering only the X axis values of the Gyroscope*/
	/*The values read from the X, Y and Z registers of the Gyroscope are in 2's complement form.*/
	/*The following algorithm converts the gyroscope values in the -250dps to +250dps range*/	

	if(raw_x > 0x7FFF)
	{
		raw_x = -(raw_x - 0x7FFF) + 1;
	}
	else
	{
		raw_x = raw_x;
	}
	
	if(raw_y > 0x7FFF)
	{
		raw_y = -(raw_y - 0x7FFF) + 1;
		ytotal = raw_y * (8.75f/1000);
	}
	else
	{
		raw_y = raw_y;
		ytotal = raw_y * (8.75f/1000);
	}
	if(raw_z > 0x7FFF)
	{
		raw_z = -(raw_z - 0x7FFF) + 1;
		ztotal = raw_z * (8.75f/1000);
	}
	else
	{
		raw_z = raw_z;
		ztotal = raw_z * (8.75f/1000);
	}


	/*Note : The sensitivity for 250 dps is 8.75 mdps/digit*/

	
}
