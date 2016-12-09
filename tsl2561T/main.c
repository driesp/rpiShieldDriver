#include <bcm2835.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	if(!bcm2835_init())
	{
	return 1;
	}
	

	char command[1];
	int val[2];
	int i = 1;
	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(0x29);
	bcm2835_i2c_set_baudrate(1000);


	command[0] = 0xa0;
       	bcm2835_i2c_write(command,1);
       	command[0] = 0x03;
       	bcm2835_i2c_write(command,1);
       	bcm2835_delay(500);
	command[0] = 0xac;
	bcm2835_i2c_write(command,1);
	bcm2835_i2c_read(command,1);
	val[1] = (int)command[0];
	command[0] = 0xad;
        bcm2835_i2c_write(command,1);
        bcm2835_i2c_read(command,1);
        val[0] = (int)command[0];
	printf("TSL:%d\n",val[0]*256+val[1]);
	
	bcm2835_i2c_end();
	bcm2835_close();
	return 0;
	
}
