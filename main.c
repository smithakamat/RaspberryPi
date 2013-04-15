#include <stdio.h>
#include "gyro.h"


void main(void)
{
	/*Initialze the hardware control registers*/	
	init();
	
	while(1)
	{
		/*Read the status of the Status register*/
		isDataRdy();

		/*Read the data*/
		read_gyro_data();

                /*The data is ready now. Process the data*/

        }



}



