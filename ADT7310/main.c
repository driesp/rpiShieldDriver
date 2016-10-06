#include <bcm2835.h>
#include <stdio.h>

int main (int argc, char **argv)
{	
	if(!bcm2835_init())
	{
	return 1;
	}
	char buffer[4] = {0,0,0,0};
	buffer[0] = 50;
	int i, temp;
	
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);

	buffer[0] = 0x58;

	bcm2835_spi_transfern(buffer,2);
	printf("id:%02X\n",buffer[1]);
	
	while(1)
	{
		buffer[0] = 0x50;					//read the temp
		bcm2835_spi_transfern(buffer,3);
		temp = buffer[1]; 
		temp = temp<<8;
                temp = temp + ( buffer[2] & 0xF8);
		temp = temp>>3;
		temp = temp/16;
		printf("temp:%d\n",temp);
		sleep(1);
	}
	
	bcm2835_spi_end();
	bcm2835_close();

	return 0;
	
}
