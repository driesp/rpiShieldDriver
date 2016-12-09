#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>

#define ledCount 8

void setOutputs();

RPiGPIOPin leds[ledCount] = {RPI_GPIO_P1_11,RPI_GPIO_P1_12,RPI_GPIO_P1_13,RPI_GPIO_P1_15,RPI_GPIO_P1_16,RPI_GPIO_P1_18,RPI_GPIO_P1_08,RPI_GPIO_P1_07};


int main (int argc, char ** argv)
{
    int parameter;
    if( argc == 2 )
    {
        printf("The argument supplied is %s\n", argv[1]);
        parameter = atoi(argv[1]);
    }
    else if( argc > 2 )
    {
        printf("Too many arguments supplied.\n");
    }
    else
    {
        printf("One argument expected.\n");
        exit(0);
    }
    if(!bcm2835_init())
    {
        return 1;
    }
    setOutputs();
    int j = 0;
    while (j <= parameter )
    {
        int i;
        for(i = 0; i < ledCount; i++)
        {
            // Turn it on
            bcm2835_gpio_write(leds[i], HIGH);
            // wait a bit
            bcm2835_delay(50);
            // turn it off
            bcm2835_gpio_write(leds[i], LOW);
        }
        for(i = 7; i >= 0; i--)
        {
            // Turn it on
            bcm2835_gpio_write(leds[i], HIGH);
            // wait a bit
            bcm2835_delay(50);
            // turn it off
            bcm2835_gpio_write(leds[i], LOW);
        }
        j++;
    }
    bcm2835_close();
    return 0;
}

void setOutputs()
{
    int i;
    for( i = 0; i < ledCount ; i++)
    {
        bcm2835_gpio_fsel(leds[i],BCM2835_GPIO_FSEL_OUTP);
    }
}
