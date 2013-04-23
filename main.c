#include <stdio.h>
#include "acc.h"


void main(void)
{
	/*Initialize the Accelerometer*/	
	initAcc();
	
	while(1)
	{
		/*Read the X,Y,Z axes values of the accelerometer*/
		readAcc_XYZ();
	
	}
	
}

