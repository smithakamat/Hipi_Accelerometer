#include <stdio.h>
#include "acc.h"
#include "gyro.h"

void main(void)
{
	/*Initialize the Accelerometer*/	
	initAcc();

	/*Initialize the gyroscope*/
	initGyro();
	
	while(1)
	{
		/*Read the X,Y,Z axes values of the accelerometer*/
		readAcc_XYZ();
		
		/*Read the X,Y,Z axes values of the gyroscope*/
		readGyro_XYZ();
	}
	
}

