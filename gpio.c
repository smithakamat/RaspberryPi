#include<stdio.h>
//#include "bcm2835.h"


#include "gpio.h"

int gpio(void)
{
    

    if (!bcm2835_init())
		return 1;

    // Set RPI pin P1-15 to be an input
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
	printf("P1-22 set up as input\n");
	
    //  with a pullup
    bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_UP);
	printf("P1-22 configured with pull up\n");

}
