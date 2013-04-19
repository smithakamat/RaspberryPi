
/******************************************************************************
 *
 * spi_basic
 * author: Tino Goehlert
 * ----------------------------------------------------------
 *    very basic SPI demo written for the Raspberry Pi.
 *    It will send and receive one byte (0xFF) over SPI.
 *
 ******************************************************************************/
/****First comment for SPI***/
 
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
 
// some defines for our SPI config
#define SPI_MODE              SPI_MODE_0    /// SPI Mode
#define SPI_BITS_PER_WORD     8       /// we're sending bytewise
#define SPI_MAX_SPEED         1000000        /// maximum speed is 1 Mhz
 
//global spi file descriptor
int spi_fd;
 
/* spi_wr_1b:
*    - sending one byte and write the received byte into the send buffer.    
*/
char spi_wr_1b(unsigned int data, 
               int delay)
{
    int ret = 0;
    struct spi_ioc_transfer spi;
 
    spi.tx_buf        = (unsigned long)&data;
    spi.rx_buf        = (unsigned long)&data;
    spi.len           = 1;
    spi.delay_usecs   = delay;
    spi.speed_hz      = SPI_MAX_SPEED;
    spi.bits_per_word = SPI_BITS_PER_WORD;
 
    if(ret = ioctl (spi_fd, SPI_IOC_MESSAGE(1), &spi) <  0){
        printf("ERROR while sending\n");
    }
    return data;
}
 
/*spi_open
*      - Open the given SPI channel and configures it.
*      - there are normally two SPI devices on your PI:
*        /dev/spidev0.0: activates the CS0 pin during transfer
*        /dev/spidev0.1: activates the CS1 pin during transfer
*
*/
int spi_open(char *dev)
{
  int _mode  = SPI_MODE;
  int _bpw   = SPI_BITS_PER_WORD;
  int _speed = SPI_MAX_SPEED;
    
  if((spi_fd = open(dev,O_RDWR)) <  0){
    printf("The file descriptor is %d\n",spi_fd);
    printf("error opening /dev/spidev0.0 \n");
    return -1;
  }else{
    printf(" opened successfully ;-)\n");
  }
 
  if (ioctl (spi_fd, SPI_IOC_WR_MODE, &_mode) <  0) 
      return -1 ;
      printf("1\n");
  if (ioctl (spi_fd, SPI_IOC_RD_MODE, &_mode) <  0) 
      return -1 ;
      printf("2\n");
  if (ioctl (spi_fd, SPI_IOC_WR_BITS_PER_WORD, &_bpw) <  0) 
      return -1 ;
      printf("3\n");
  if (ioctl (spi_fd, SPI_IOC_RD_BITS_PER_WORD, &_bpw) <  0) 
      return -1 ;
      printf("4\n");
 
  if (ioctl (spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &_speed) <  0) 
      return -1 ;
      printf("5\n");
  if (ioctl (spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &_speed) <  0) 
      return -1 ;
 
  return 0;
 
}
 
int main(int argc, char* argv[])
{
    unsigned int data = 0xAAAA;
 /*
    if(argc <  1){
        printf("too few args, try %s /dev/spidev0.0\n",argv[0]);
        return -1;
    }
 */
    // open and configure SPI channel. (/dev/spidev0.0 for example)
    if((spi_open(argv[1])) <  0){
        printf("spi_open failed\n");
        return -1;
    }
 
    // send one Byte (0xAAAA) - receive one byte. 
    spi_wr_1b(data,0);
    printf("RECEIVED: %.2X\n",data);
 
    // close SPI channel
    close(spi_fd);
 
    return 0;
}

