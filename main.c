#include <stdio.h>
#include "acc.h"
#include "gyro.h"

void main(void)
{
      

	/*Initialize the Gyroscope*/
        initGyro();
	/*Initialize the Accelerometer*/	
	initAcc();



       
	
	while(1)
	{
		/*Read the X,Y,Z axes values of the accelerometer*/
		readAcc_XYZ();
		
		sleep(1);		
		/*Read the X,Y,Z axes values of the gyroscope*/
		readGyro_XYZ();
	}
	
}

