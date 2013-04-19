#include <stdio.h>
#include "gyro.h"


int main()
{
	unsigned int tt,devIdentify;

        sleep(2);

	/*Open and configure SPI channel --> /dev/spidev0.0 */
   	spi_open();
	
	/*Check for device identification of the Gyroscope*/	
	tt = WHO_AM_I | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
        spi_wr_1b(tt,0);       		      /*Set the address of the WHO AM I register*/
	devIdentify=spi_wr_1b(0x00,0);        /*Read the value at that address*/
	if((devIdentify && 0xD3) == 1)
		printf("Gyro identification successfull\n");
	else 
		printf("Gyro identification failed\n");
	/*Initialze the hardware control registers*/	
	init();
	
	while(1)
	{
		/*Read the status of the Status register*/
		isDataRdy();

		/*Read the data*/
		read_gyro_data();
		sleep(1);

                /*The data is ready now. Process the data*/

        }

	close(spi_fd);

}


