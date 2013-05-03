/*Description: This code utilizes the angular velocity of the gyroscope along the X axis and the 
velocity sensed by the acclerometer along the Y axis and implements the digital complementary filter 
to calculate the appropriate tilt angle and also to remove drift and other associted noise of the sensors.
The tilt angles and the timing parameter are then fed to the GNUPLOT and the disrection of motion is plotted

The follwoing code plots the tilt angle as a function of time only for the first 10 values obtained from
the gyroscope

Authors: Smitha Sunil Kamat, Jay Khandhar
*/

/*Preprocessor directives and user defined header files*/
#include <stdio.h>
#include "acc.h"
#include "gyro.h"
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

/*Declaration of macros*/

/*The GYRO_CONST is the gyroscope complementary filter co-efficient
The ACC_CONST is the acceleromter complementary filter co-efficient*/
#define GYRO_CONST 0.699f
#define ACC_CONST  0.301f
#define COMMENT 1

void main(void)
{
  	/*Declaration of Local variables and structures*/
	struct timespec prev, current;
	extern struct timespec gyro_time;
	extern unsigned int raw_x, raw_y, raw_z; /*Raw digital values of the angular velocities of the gyroscope*/
	extern unsigned int conv_raw_xconcat,conv_raw_yconcat,conv_raw_zconcat;   /*Raw positive and negative angular velocity values of the accelerometer */
	long nano_sec, sec, concat_time; 
	float overall_angle = 0; /*Variable to store the overall tilt angle*/
	int i = 0;
	unsigned int j = 0;	
	double xval[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; /*Array storing the x axis co-ordinate values(tilt angle) for the plot*/
	double yval[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; /*Array storing the y axis co-ordinate values(time) for the plot*/
	double plot_time = 0.0; /*Temporary Variable to store the incremental y axis co-ordinates for the plot*/

	unsigned int avg_count = 0; /*Count variable to calculate the average of the first 10 raw_x gyro values*/
	unsigned int avg_raw_x = 0; /*Temp variable to hold the average of the first 10 raw_x gyro values*/	

	printf("------------------------------------Initializing hardware---------------------------------------\n");

	/*Initialize the Gyroscope*/
        initGyro();
	
	/*Obtain the first 10 raw angular velocity values along the x axis from the gyro*/
	/*Subtract the average of the 10 values from the gyro from the subsequent read values*/
	do{
		readGyro_XYZ();
		avg_raw_x = avg_raw_x + raw_x;
		avg_count++;

	}while(avg_count != 10);

	/*Store the average value in a temporary variable*/
	avg_raw_x = avg_raw_x/10;

	/*Initialize the Accelerometer*/	
	initAcc();

	printf("---------------------------------Initialization of the hardware complete-----------------------------------\n");

	/*Obtain the timestamp when the last x angular velocity value was obtained*/
        clock_gettime(CLOCK_MONOTONIC, &prev);
	
	do
	{
		/*Read the X,Y,Z axes values of the accelerometer*/
		readAcc_XYZ();
	
	
		/*Read the X,Y,Z axes values of the gyroscope*/
		readGyro_XYZ();

		/*Neglect negative angular velocity values beyond 20000*/
		if(abs(raw_x) >= abs(-20000))
		{	
			raw_x = 0;
		}
	
		raw_x = raw_x - avg_raw_x;

		/*Update the current timestamp*/
		current = gyro_time;


		/*Calculating the time between two consecutive gyro angle reads*/
		if((current.tv_nsec - prev.tv_nsec) < 0)
		{
			nano_sec = 1000000000 + current.tv_nsec - prev.tv_nsec;
			sec = current.tv_sec - prev.tv_sec - 1;
		}		
		else
		{
			nano_sec = current.tv_nsec - prev.tv_nsec;
			sec = current.tv_sec - prev.tv_sec;
		}

		concat_time = sec + (nano_sec/1000000000);
		memcpy((unsigned char *)&prev,(unsigned char *)&current,sizeof(struct timespec));

		printf("Raw Gyro Value : %d\n",raw_x);
		
		plot_time = plot_time + concat_time;  /*Update the x axes values*/
		
#if COMMENT	
		
		/*-------------------------------Complementary filter implementation---------------------------------------*/
		/*The part of the complementary filter equation involving the gyro values and the gyro constant represent the 
		integrator/high pass filter and the part of the equation involving the accleromter constant and the accelerometer
		value reperesents the low pass filter*/

		/*Implementing the switch case to take care of +/-, -/+, -/- and +/+ conditions*/
   		/*the variable raw_x represents the +/- raw gyro values, the variable conv_raw_yconcat represents the raw +/- Accelerometer values */
		if (raw_x <= 0x7FFF)
		{
                        /*Gyro angle is positive*/
			float gyro_angle=0;
			gyro_angle= (float) raw_x * 8.75f/1000;
			if(conv_raw_yconcat<=0x7FF )
			{	
				/*Get the Accelerometer reading in terms of g*/
   				AccG_Y= (float) conv_raw_yconcat *(float) GSCALE /2048.0;
				/*Accelerometer is positive, but we need negative y */
				overall_angle= ((GYRO_CONST) * (overall_angle + (gyro_angle * concat_time))) - ((ACC_CONST)*AccG_Y);
			}
			else
			{
				/*The Accelerometer value is negative*/
			        /*Before converting it to g, first make the raw value positive, we'll adjust the negative sign in the
				complemantary filter equation*/
				conv_raw_yconcat = conv_raw_yconcat *( -1);
				AccG_Y = (float) conv_raw_yconcat *(float) GSCALE/2048.0;
				overall_angle = ((GYRO_CONST) * (overall_angle + (gyro_angle * concat_time))) + ((ACC_CONST)*AccG_Y);
			}
		}
		else
		{
			/*Gyro angle is negative*/
			float gyro_angle=0;
			/*Make the gyro raw value positive, we will adjust the negative sign in the complementary filter equation*/
			raw_x = raw_x * (-1);
			gyro_angle =(float) raw_x * 8.75f/1000;
			if(conv_raw_yconcat<=0x7FF)
			{
				/*Get the Accelerometer reading in terms of g*/
				AccG_Y= (float) conv_raw_yconcat * (float)GSCALE/2048.0;
				/*Accelerometer value is positive but we need negative y*/
				overall_angle= - ((GYRO_CONST)* (overall_angle + (gyro_angle * concat_time))) -((ACC_CONST) * AccG_Y);
			}
			else
			{
				/*The Accelerometer value is negative*/
				/*First make it positive*/
				conv_raw_yconcat= conv_raw_yconcat * (-1);
				AccG_Y= (float) conv_raw_yconcat *(float) GSCALE/2048.0;
				overall_angle= ( (GYRO_CONST) * (overall_angle + (gyro_angle * concat_time))) + (ACC_CONST * AccG_Y);
			}
                 }
		printf("The tilt angle is %f\n",overall_angle);
#endif		

		xval[j] = plot_time;
		yval[j] = overall_angle;
		j++;
	}while(j!= 10);


		/*--------------------------------------Code for plotting the graph using GNUPLOT----------------------------*/
		
		/*Charcter pointer to the plot*/
		char * commandsForGnuplot[] = {"set title \"Plot of Tilt angle (y axis) v/s Time (x axis)\"", "plot 'val.txt'"};
	
		/*Create a text file in the write mode to store the tilt anle and the time values*/
		FILE * fp = fopen("val.txt","w"); 

		/*Open a pipe to communicate with GNUPLOT*/
		FILE * gnuplotpipe = popen("gnuplot -persistent", "w");

		/*Write the tilt angle values in the text file*/
		for(i = 0; i<10; i++)
		{
			fprintf(fp, "%lf %lf\n",xval[i],yval[i]);
		}

		fclose(fp);

		printf("-----------------Plotting the graph---------------------\n");
		for(i =0; i<2 ;i++)
		{
			/*Send commands to the Gnuplot one by one*/
			fprintf(gnuplotpipe, "%s \n", commandsForGnuplot[i]);
		}
		fflush(gnuplotpipe);	
//#endif 
	
}

