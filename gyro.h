#ifndef GYRO_H_
#define GYRO_H_

/*Defining masks*/
#define SPI_READ_MASK 0x80
#define SPI_WRITE_MASK 0x00
#define SPI_ADDR_UNCHANGED 0x00
#define SPI_ADDR_AUTOINC 0x40

/*Defining the SPI parameters*/
#define SPI_MODE          SPI_MODE_0
#define SPI_MAX_SPEED     1000000
#define SPI_BITS_PER_WORD 8

/*************************************************************************************************
****Definition of the addresses of the of the hardware registers in L3G4200D**********************
**************************************************************************************************/

/*Defining the address of the WHO_AM_I device identification register*/
#define WHO_AM_I 0x0F

/*Defining the address of control registers*/
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

/*Defining the address of output registers for yaw, pitch and roll*/
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

/*Defining the address of the status register*/
#define STATUS_REG 0x27

/*Defining the address of the fifo control register*/
#define FIFO_CTRL_REG 0x2E

int spi_fd;

unsigned char spi_wr_1b(unsigned char data, int delay);
int spi_open(void);
void init(void);
void isDataRdy(void);
void read_gyro_data(void);


#endif /*GYRO_H_*/
