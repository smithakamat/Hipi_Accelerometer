#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>   
#include "gyro.h"
#include "i2c_rw.h"
#include <time.h>


/*Initialization sequence of the gyroscope*/
void initGyro(void)
{
	unsigned char temp,dev_identify;
	//sleep(1);
	/*Read the device identification register of the Gyroscope*/
	dev_identify = read_reg(0x6B,0x0F);
	printf("The dev ID if gyro is %d\n", dev_identify);
	if(temp == 0xD4 )
		printf("The Gyroscope is on I2C bus - 1\n");
	else
		printf("ERROR !!! - Gyroscope  not found on I2C bus - 1\n");

	
	/*Write into CTRL_REG1 in order to enable x, y and z axes output of the gyro*/
	temp = 0x0F;
	write_reg(gyro_slvAddr,CTRL_REG1,temp);
	
	//sleep(1);
	/*Disable the high pass filter of the gyro*/
	write_reg(gyro_slvAddr,CTRL_REG2,0x00);
	
	//sleep(1);	
	/*Enabling the DRDY pin of the gyro*/
	temp = 0x08;
	write_reg(gyro_slvAddr,CTRL_REG3,temp);
	
	//sleep(1);
	/*Initializing CTRL_REG4*/
	write_reg(gyro_slvAddr,CTRL_REG4,0x00);	

	//sleep(1);
	/*Initializing CTRL_REG5*/
	write_reg(gyro_slvAddr,CTRL_REG5,0x00);

	sleep(1);
       

	/*Bring the gyro out of power down mode*/
	temp = 0x0F;
	write_reg(gyro_slvAddr,CTRL_REG1,temp);

        
	
}

/*Reading the X,Y,Z axes values of the gyroscope*/
void readGyro_XYZ(void)
{

	unsigned char xlow=0, xhigh=0, ylow=0, yhigh=0, zlow=0, zhigh=0;
	float xtotal=0, ytotal=0, ztotal=0;
	unsigned int raw_x, raw_y, raw_z;
	
	clock_t start,stop;

	float elapsedTime;
	
	start = clock();

	xlow = read_reg(gyro_slvAddr,OUT_X_Lo);
	xhigh = read_reg(gyro_slvAddr,OUT_X_Hi);
	
	ylow = read_reg(gyro_slvAddr,OUT_Y_Lo);
	yhigh = read_reg(gyro_slvAddr,OUT_Y_Hi);
	
	zlow = read_reg(gyro_slvAddr,OUT_Z_Lo);
	zhigh = read_reg(gyro_slvAddr,OUT_Z_Hi);

	raw_x = (xhigh << 8) + xlow;
	raw_y = (yhigh << 8) + ylow;
	raw_z = (zhigh << 8) + zlow;
	
	stop = clock();
	printf("----------------------------------------------------\n");
	printf("X : Y : Z : %d, %d, %d\n", raw_x, raw_y, raw_z);

	if(raw_x > 0x7FFF)
	{
		raw_x = -(raw_x - 0x7FFF) + 1;
		xtotal = raw_x * (8.75f/1000);
	}
	else
	{
		raw_x = raw_x;
		xtotal = raw_x * (8.75f/1000);
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
	
	//stop = clock();


	elapsedTime = (stop - start);   //time in milliseconds

	printf("The elapsed time is %ld seconds\n", elapsedTime);

	/*The sensitivity for 250 dps is 8.75 mdps/digit*/
	//xtotal = raw_x * (8.75/1000);
	//ytotal = raw_y * (8.75/1000);
	//ztotal = raw_z * (8.75/1000);
	
	printf("Raw_Gyro_X : Raw_Gyro_Y : Raw_Gyro_Z : %d, %d, %d\n", raw_x, raw_y, raw_z);
	printf("Gyro_X : Gyro_Y : Gyro_Z : %f, %f, %f\n", xtotal, ytotal, ztotal);		
}
