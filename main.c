#include <stdio.h>
#include "gyro.h"
#include "gpio.h"
//#include "bcm2835.h"
int main()
{
	unsigned int tt,devIdentify;
	/*Calling the gpio function to configure GPIO 22 as input*/
	gpio();
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
		//isDataRdy();
		/*Polling the data ready pin*/
		uint8_t  value = bcm2835_gpio_lev(PIN);
		
		printf("The value on the data ready pin is %d\n",value);
		
		while(!value);
		//if(value == 1)
		{
			/*Read the data*/
			read_gyro_data();
			sleep(1);
		}
                /*The data is ready now. Process the data*/
		
        }

	close(spi_fd);

}


