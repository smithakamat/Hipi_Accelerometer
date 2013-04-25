#include <stdio.h>
#include "acc.h"
#include "gyro.h"
#include <time.h>

void main(void)
{
      
	struct timespec prev, current;
	extern struct timespec gyro_time;
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



		/*Calculating the angles*/
		overall_angle = ((0.98f) * (overall_angle + (xtotal * concat_time))) + ((0.02f) * AccG_X);
		printf("The overall angle is %f\n", overall_angle);
		

	}
	
}

