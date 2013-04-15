/*********************Header files****************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
//#include <linux/delay.h> /*the compiler is not accepting this header file - fatal error 

/*User-defined Headers*/
#include "gyro.h"


/*Defining global variables*/
unsigned int temp, read_status;
unsigned int xlow, xhigh, ylow, yhigh, zlow, zhigh;


/*Declaring the SPI file descriptor*/
int spi_fd;

/*Sending 1byte and write the received byte into the send buffer*/
unsigned int spi_wr_2b(unsigned int data, int delay)
{
	int ret = 0;
	struct spi_ioc_transfer spi;
	
	spi.tx_buf = (unsigned long)&data;
	spi.rx_buf = (unsigned long)&data;
	spi.len = 2;
	spi.delay_usecs = delay;
	spi.speed_hz = SPI_MAX_SPEED;
	spi.bits_per_word = SPI_BITS_PER_WORD;

	if(ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi) <  0)
	{
		printf("\n\rError while sending the byte");
		printf("\n\rThe data transmitted is %x", data);
	}
	
	return data;
	
	
}

/*Function to initialize the control registers with their initial values*/
void init(void)
{
	unsigned int data;

	/*Turn on the gyro sensor, place it in normal mode and enable X,Y and Z*/
	/*ODR = 100Hz, Low pass filter1 (LPF1) cut off = 32Hz, Low pass filter2 (LPF2) cut off = 12.5Hz*/
	temp = CTRL_REG1 | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	temp = (temp << 8) + 0x0F;
	spi_wr_2b(temp,0);
		
	/*High pass filter has been disabled*/
	temp = CTRL_REG2 | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	temp = (temp << 8) + 0x00;
	spi_wr_2b(temp,0);
	
	/*Disabling INT1 of the gyro sensor*/
	temp = CTRL_REG3 | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	temp = (temp << 8) + 0x00;
	spi_wr_2b(temp,0);

	/*Little endian mode, Continuous block data update, Full scale = 250dps, Selft test disabled, SPI 4 wire interface*/
	temp = CTRL_REG4 | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	temp = (temp << 8) + 0x00;
	spi_wr_2b(temp,0);

	/*Enable FIFO, Low pass filter1 (LPF1) and Low pass filter2 (LPF2)*/
	/*High pass filter (HPF) disabled*/
	temp = CTRL_REG5 | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	temp = (temp << 8) + 0x42;
	spi_wr_2b(temp,0);

	/*Configure the FIFO in Stream mode*/
	temp = FIFO_CTRL_REG | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	temp = (temp << 8) + 0x40;
	spi_wr_2b(temp,0);

	/*Provide a delay of 5ms for the gyro sensor to write va;ues from flash to the hardware registers*/
	//mdelay(5); //CHECK IF mdelay() works???
	sleep(1);

	/*Pull the gyro sendor out of power down mode to normal mode*/
	temp = CTRL_REG1 | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	temp = (temp << 8) + 0x0F;
	spi_wr_2b(temp,0);
}

void isDataRdy(void)
{
		
	temp = STATUS_REG | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
	temp = (temp << 8);
	read_status = spi_wr_2b(temp,0);

}
void read_gyro_data(void)
{
              
	       unsigned int temp;

               if((read_status && 0x04) == 1)
		{
			temp = OUT_X_L | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
			temp = (temp << 8);
			xlow = spi_wr_2b(temp,0);

			temp = OUT_X_H | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
			temp = (temp << 8);
			xhigh = spi_wr_2b(temp,0);

			temp = OUT_Y_L | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
			temp = (temp << 8);
			ylow = spi_wr_2b(temp,0);

			temp = OUT_Y_H | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
			temp = (temp << 8);
			yhigh = spi_wr_2b(temp,0);

			temp = OUT_Z_L | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
			temp = (temp << 8);
			zlow = spi_wr_2b(temp,0);
	
			temp = OUT_Z_H | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
			temp = (temp << 8);
			zhigh = spi_wr_2b(temp,0);
		}
		else
		{	
			isDataRdy();
                }
}



















        

