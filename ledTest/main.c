#include <bcm2835.h>
#include <stdio.h>

#define PIN RPI_GPIO_P1_11

int main (void)
{
	if(!bcm2835_init())
	{
		return 1;
	}
	bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
	while (1)
    	{
        	// Turn it on
        	bcm2835_gpio_write(PIN, HIGH);
        
        	// wait a bit
        	bcm2835_delay(500);
        
        	// turn it off
        	bcm2835_gpio_write(PIN, LOW);
        
        	// wait a bit
        	bcm2835_delay(500);
    	}
    bcm2835_close();
    return 0;
}