#include <stdio.h>
#include "acc.h"
#include "gyro.h"
#include <time.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

#define GYRO_CONST 0.98f
#define ACC_CONST  0.02f
#define COMMENT 0

void main(void)
{
      
	struct timespec prev, current;
	extern struct timespec gyro_time;
	extern unsigned int raw_x, raw_y, raw_z; /*Raw digital values of the gyroscope*/
	extern unsigned int conv_raw_xconcat,conv_raw_yconcat,conv_raw_zconcat;   /*Raw +/-  values of the accelerometer */
	long nano_sec, sec, concat_time;
	float overall_angle = 0;

	/*Initialize the Gyroscope*/
        initGyro();

	/*Initialize the Accelerometer*/	
	initAcc();



       clock_gettime(CLOCK_MONOTONIC, &prev);
	
	while(1)
	{
		/*Read the X,Y,Z axes values of the accelerometer*/
		readAcc_XYZ();

	
		/*Read the X,Y,Z axes values of the gyroscope*/
		readGyro_XYZ();

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


		/*Calculating the angles*/
        	      /* Original Complementary filter equation
		overall_angle = ((0.98f) * (overall_angle + (xtotal * concat_time))) + ((0.02f) * AccG_X);
		printf("The overall angle is %f\n", overall_angle);*/
		
		printf("Raw Gyro Value : %d\n",raw_x);
		printf("Time in secs is: %ld\n",concat_time);
		
#if COMMENT	

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
		printf("The angle is %f\n",overall_angle);

#endif 
				


	}
	
}

