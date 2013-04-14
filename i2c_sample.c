#include <linux/i2c-dev.h>
#include<linux/i2c.h>

/*I2C driver code*/
int deviceHandle;
char buffer[7];

int main(void)

/* initialize the buffer*/
buffer = 0x00;

/*open the i2c device on /dev/i2c-0*/
deviceHandle = open("/dev/i2c-0",O_RDWR);

/*connect to the accelerometer as the slave*/
//int accI2Caddr = ;
ioctl(deviceHandle,I2C_SLAVE,accI2Caddr);

/*begin the transmission and request for acknowledgement*/
write(deviceHandle,buffer,1);

/*close connection and return*/
close(deviceHandle);
return 0;
}

