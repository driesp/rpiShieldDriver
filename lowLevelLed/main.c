#include <stdio.h>
#include  <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

uint32_t *bcm2835_peripherals = (uint32_t *)MAP_FAILED;
uint32_t *bcm2835_peripherals_base = (uint32_t *)0x20000000;
uint32_t bcm2835_peripherals_size = 0x01000000;

volatile uint32_t *gpio = (uint32_t *)MAP_FAILED;
volatile uint32_t *pwm = (uint32_t *)MAP_FAILED;
volatile uint32_t *clk = (uint32_t *)MAP_FAILED;
volatile uint32_t *pads = (uint32_t *)MAP_FAILED;
volatile uint32_t *spi0 = (uint32_t *)MAP_FAILED;
volatile uint32_t *bsc0 = (uint32_t *)MAP_FAILED;
volatile uint32_t *bsc1 = (uint32_t *)MAP_FAILED;
volatile uint32_t *st = (uint32_t *)MAP_FAILED; 

static void *mapmem(const char *msg, size_t size, int fd, off_t off)
{
	void *map = mmap(NULL, size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, off);
	return map;
}

void writeP( volatile uint32_t *paddr, uint32_t value)
{
         __sync_synchronize();
         *paddr = value;
         __sync_synchronize();
}
void setP(uint8_t pin)
{
        volatile uint32_t *paddr  = gpio + 0x001C/4 + pin/32;
        uint8_t shift = 17 % 32;
        writeP(paddr, 1 << shift);
}
void clearP(uint8_t pin)
{
        volatile uint32_t *paddr  = gpio + 0x0028/4 + pin/32;
        uint8_t shift = 17 % 32;
        writeP(paddr, 1 << shift);
}


int main(int argc, char **argv)
{
	FILE *fp;
	int memfd =  -1;
	if((fp = fopen("/proc/device-tree/soc/ranges", "rb")))
	{
		unsigned char buf[4];
		fseek (fp,4,SEEK_SET);
		if(fread(buf, 1, sizeof(buf), fp) == sizeof(buf))
		{
			bcm2835_peripherals_base = (uint32_t *)(buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3] << 0);
		}
		fseek(fp,8,SEEK_SET);
		if(fread(buf,1,sizeof(buf),fp) == sizeof(buf))
		{
			bcm2835_peripherals_size = (buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3] << 0);
		}
		fclose(fp);
		printf("Read The File!\n");
	}
	if(geteuid() == 0)
	{
		if(memfd = (open("/dev/mem", O_RDWR | O_SYNC)))
		{
			printf("can open mem!\n");
		}
		bcm2835_peripherals = mapmem("gpio", bcm2835_peripherals_size, memfd, (uint32_t)bcm2835_peripherals_base);
		gpio = bcm2835_peripherals + 0x200000/4;
		pwm = bcm2835_peripherals + 0x20C000/4;
		clk = bcm2835_peripherals + 0x101000/4;
		pads = bcm2835_peripherals + 0x100000/4;
		spi0 = bcm2835_peripherals + 0x204000/4;
		bsc0 = bcm2835_peripherals + 0x205000/4;
		bsc1 = bcm2835_peripherals + 0x804000/4;
		st = bcm2835_peripherals + 0x3000/4;
	}
	close(memfd);


	while(1)
	{
		struct timespec sleeper;
                sleeper.tv_sec = (time_t)(1000/1000);
                sleeper.tv_nsec = (long)(1000%1000)*1000000;

		setP(17);
		printf("Set Pin!\n");

		nanosleep(&sleeper, NULL);

		clearP(17);
		printf("Cleared Pin!\n");

		nanosleep(&sleeper, NULL);

    	} 
}

