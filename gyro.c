
/********************************************************
***********************Header files**********************
*********************************************************/

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
//#include <linux/delay.h> /*the compiler is not accepting this header file - fatal error 

/*User-defined Headers*/
#include "gyro.h"


/*Defining global variables*/
unsigned int temp, read_status;
unsigned int xlow, xhigh, ylow, yhigh, zlow, zhigh;
unsigned int xtotal, ytotal, ztotal;
//unsigned int devIdentify;

unsigned int ctrlreg1_val, ctrlreg2_val;

/*Declaring the SPI file descriptor*/
int spi_fd;

/*Sending 1 byte and writing the received byte into the send buffer*/
unsigned int spi_wr_1b(unsigned int data, int delay)
{
	int ret = 0;
	struct spi_ioc_transfer spi;
	
	spi.tx_buf = (unsigned long)&data;
	spi.rx_buf = (unsigned long)&data;
	spi.len = 1;
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

/*spi_open
*      - Open the given SPI channel and configure it.
*      - there are normally two SPI devices on your PI:
*        /dev/spidev0.0: activates the CS0 pin during transfer
*        /dev/spidev0.1: activates the CS1 pin during transfer
*
*/
int spi_open(void)
{
  	int _mode  = SPI_MODE;
  	int _bpw   = SPI_BITS_PER_WORD;
  	int _speed = SPI_MAX_SPEED;
 
  	if((spi_fd = open("/dev/spidev0.0", O_RDWR)) < 0)
	{
	    printf("Error opening the spi device\n\r");
	    return -1;
	}
	else
	{
	    printf("The SPI device has been opened successfully\n\r");
	}
	 
	 if (ioctl (spi_fd, SPI_IOC_WR_MODE, &_mode) < 0)
	     return -1 ;

	 if (ioctl (spi_fd, SPI_IOC_RD_MODE, &_mode) < 0)
	     return -1 ;
	 
	 if (ioctl (spi_fd, SPI_IOC_WR_BITS_PER_WORD, &_bpw) < 0)
	     return -1 ;

	 if (ioctl (spi_fd, SPI_IOC_RD_BITS_PER_WORD, &_bpw) < 0)
	     return -1 ;
	 
	 if (ioctl (spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &_speed) < 0)
	     return -1 ;

	 if (ioctl (spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &_speed) < 0)
	     return -1 ;
	 
	 return 0;
	 
}

/*Function to initialize the control registers with their initial values*/
void init(void)
{
	unsigned int data;

	/*Turn on the gyro sensor, place it in normal mode and enable X,Y and Z*/
	/*ODR = 100Hz, Low pass filter1 (LPF1) cut off = 32Hz, Low pass filter2 (LPF2) cut off = 12.5Hz*/

	temp = CTRL_REG1 | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	/*Write the address of CTRL_REG1*/
	spi_wr_1b(temp,0);
	printf("The address set for CTRL_REG1 is %d\n", temp);
	/*Write data into CTRL_REG1*/
	temp = 0x0F;
        spi_wr_1b(temp,0);
	printf("The value written into  CTRL_REG1 is %d\n", temp);
	temp= CTRL_REG1 | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
	printf("The address set before reading is %d\n", temp);
        /*Set the address*/
 	 spi_wr_1b(temp,0);
        /*Read the value at that address*/
        ctrlreg1_val=spi_wr_1b(temp,0);
	printf("The value of CTRL_REG1 is %d\n", ctrlreg1_val);

	

	/*High pass filter has been disabled*/

	temp = CTRL_REG2 | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	/*Set the address of CTRL_REG2*/
	spi_wr_1b(temp,0);
	temp = 0x00;
        /*Write the value at that address*/
	spi_wr_1b(temp,0);
	
	/*Set the address from where we want to read*/
	temp= CTRL_REG2 | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
        spi_wr_1b(temp,0);
        /*Read value at that address*/
	ctrlreg2_val=spi_wr_1b(0x00,0);
	printf("The value of CTRL_REG2 is %d\n", ctrlreg2_val);
	
	/*Disabling INT1 of the gyro sensor*/
	temp = CTRL_REG3 | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	/*Set the address of CRL_REG3*/
	spi_wr_1b(temp,0);
	temp = 0x00;
	/*Write data into CTRL_REG3*/
	spi_wr_1b(temp,0);

	/*Little endian mode, Continuous block data update, Full scale = 250dps, Selft test disabled, SPI 4 wire interface*/
	temp = CTRL_REG4 | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	/*Set the address of CTRL_REG4*/
	spi_wr_1b(temp,0);
	temp = 0x00;
	/*Write data into CTRL_REG4*/
	spi_wr_1b(temp,0);

	/*Enable FIFO, Low pass filter1 (LPF1) and Low pass filter2 (LPF2)*/
	/*High pass filter (HPF) disabled*/
	temp = CTRL_REG5 | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	/*Set the address of CTRL_REG5*/
	spi_wr_1b(temp,0);
	temp = 0x42;
	/*Write the data into CTRL_REG5*/
	spi_wr_1b(temp,0);

	/*Provide a delay of 5ms for the gyro sensor to write va;ues from flash to the hardware registers*/
	//mdelay(5); //CHECK IF mdelay() works???
	sleep(1);
	
	/*Pull the gyro sendor out of power down mode to normal mode*/
	temp = CTRL_REG1 | SPI_WRITE_MASK | SPI_ADDR_UNCHANGED;
	/*Set the address of CTRL_REG1*/
	spi_wr_1b(temp,0);
	temp = 0x0F;
	/*Write teh data into CTRL_REG1*/
	spi_wr_1b(temp,0);
}

void isDataRdy(void)
{
		
	temp = STATUS_REG | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
	/*Set the address*/
        spi_wr_1b(temp,0);
        /*Read value at that address*/
        read_status = spi_wr_1b(0x00,0);

}
void read_gyro_data(void)
{
              
	      // unsigned int temp;
	       
	      /*Check if the device identification register of the gyroscope has the right value*/
	      // temp = WHO_AM_I | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
	      // devIdentify = spi_wr_1b(temp,0);
	      // if((devIdentify && 0xD3) == 1)
		//	printf("Gyro identification successfull !!! \n");

	       isDataRdy();
               if((read_status && 0x04) == 1)
		{
			temp = OUT_X_L | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
		        /*Set the address*/
			spi_wr_1b(temp,0);
			/*Read the value at that address*/
			xlow = spi_wr_1b(temp,0);

			temp = OUT_X_H | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
		        /*Set the address*/
                        spi_wr_1b(temp,0);
                        /*Read the value at that address*/
			xhigh = spi_wr_1b(temp,0);
		        	
			/*Concatenate high byte and low byte of x and store the 16 bit value*/
			xtotal = (xhigh << 8) + xlow;

			temp = OUT_Y_L | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
			/*Set the address*/
			spi_wr_1b(temp,0);
                        /*Read the value*/
			ylow = spi_wr_1b(temp,0);

			temp = OUT_Y_H | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
			/*Set the address*/
			spi_wr_1b(temp,0);
			/*Read Value*/
			yhigh = spi_wr_1b(temp,0);

			/*Concatenate high byte and low byte of y and store the 16 bit value*/
			ytotal = (yhigh <<  8) + ylow;

			temp = OUT_Z_L | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
			/*Set the address*/
			spi_wr_1b(temp,0);
			/*Read the value*/
			zlow = spi_wr_1b(temp,0);
	
			temp = OUT_Z_H | SPI_READ_MASK | SPI_ADDR_UNCHANGED;
			/*Set the address*/
			spi_wr_1b(temp,0);
			/*Read the value*/
			zhigh = spi_wr_1b(temp,0);

			/*Concatenate high byte and low byte of z and store the 16 bit value*/
			ztotal = (zhigh << 8) + zlow;

		
			printf("The x, y and z co-ordinate values are %5d | %5d | %5d \n", xtotal, ytotal, ztotal);
		}
		else
		{	
			isDataRdy();
                }
}



















        
