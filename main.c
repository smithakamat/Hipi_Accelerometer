#include <stdio.h>
#include "acc.h"
#include "gyro.h"
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define GYRO_CONST 0.699f
#define ACC_CONST  0.301f
#define COMMENT 1

void main(void)
{
      
	struct timespec prev, current;
	extern struct timespec gyro_time;
	extern unsigned int raw_x, raw_y, raw_z; /*Raw digital values of the gyroscope*/
	extern unsigned int conv_raw_xconcat,conv_raw_yconcat,conv_raw_zconcat;   /*Raw +/-  values of the accelerometer */
	long nano_sec, sec, concat_time;
	float overall_angle = 0;
	//FILE *fp;/*File pointer to a file to store tilt angle values*/
	int i = 0;
	unsigned int j = 0;	
	double xval[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};/*Array storing the x axis co-ordinate values for the plot*/
	double yval[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};	/*Array storing the y axis co-ordinate value for the plot*/
	double plot_time = 0.0; /*Temporary Variable to store the y axis co-ordinates for the plot*/

	//unsigned int raw_temp=0;
	//float tempC=0.0;
	unsigned int avg_count = 0; /*Count variable to calculate the average of the first 10 raw_x gyro values*/
	unsigned int avg_raw_x = 0; /*Temp variable to hold the average of the first 10 raw_x gyro values*/	

	printf("Initializing hardware\n");

	/*Initialize the Gyroscope*/
        initGyro();
	
	
	do{
		readGyro_XYZ();
		//printf("The %d raw_x value is %d\n", avg_count, raw_x);
		avg_raw_x = avg_raw_x + raw_x;
		//printf("The average value of raw_x after iteration %d is %d\n", avg_count, avg_raw_x);
		avg_count++;

	}while(avg_count != 10);

	avg_raw_x = avg_raw_x/10;
	//printf("The final average raw_x value of 10 samples is %d\n", avg_raw_x);
	

	/*Initialize the Accelerometer*/	
	initAcc();

	printf("Initialization of the hardware complete\n");

       clock_gettime(CLOCK_MONOTONIC, &prev);
	
	do
	{
		/*Read the X,Y,Z axes values of the accelerometer*/
		readAcc_XYZ();
	
	
		/*Read the X,Y,Z axes values of the gyroscope*/
		up:
		readGyro_XYZ();
		if(abs(raw_x) >= abs(-20000))
		{	
			//printf(" Negative values of raw_x %d\n", raw_x);
			raw_x = 0;
			//goto up;
		}
	
		raw_x = raw_x - avg_raw_x;
		//printf("The averaged out raw_x gyro values are %d\n", raw_x);

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
		//printf("Time in secs is: %ld\n",concat_time);
		
		plot_time = plot_time + concat_time;  /*Update the x axes values*/
		
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
		printf("The tilt angle is %f\n",overall_angle);
#endif		

		//unsigned int j = 0;
		//double xval[3];
		xval[j] = plot_time;
		yval[j] = overall_angle;
		j++;
}while(j!= 10);
		/*Code for plotting*/
		
		char * commandsForGnuplot[] = {"set title \"Plot of Tilt angle (y axis) v/s Time (x axis)\"", "plot 'val.txt'"};
		//double xval[5] = {1.0, 2.0, 3.0, 4.0};
		//double yval[3] = {1.0, 2.0, 3.0};
		FILE * fp = fopen("val.txt","w"); /*Create a file called values.txt in the current directory and open it in append mode*/
	
		printf("File is being writen into\n");

		/*Open an interface to communicate with GNUPLOT*/
		FILE * gnuplotpipe = popen("gnuplot -persistent", "w");

		/*Write the tilt angle values in a temporary file*/
		printf("Writing values to the file values.txt\n");
	
		for(i = 0; i<10; i++)
		{
			fprintf(fp, "%lf %lf\n",xval[i],yval[i]);
		}

		fclose(fp);

		/*plot the graph!!!*/
		printf("Plotting\n");
		for(i =0; i<2 ;i++)
		{
			/*Send commands to the Gnuplot one by one*/
			fprintf(gnuplotpipe, "%s \n", commandsForGnuplot[i]);
		}
		fflush(gnuplotpipe);	
//#endif 
				



	
}

