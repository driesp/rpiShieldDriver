#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <asm/uaccess.h>

#define MODULE_NAME "procdriver :"
#define IO_OFFSET 0x200000
#define GPIO 4

volatile void  *iomap;
struct proc_dir_entry *procFile;
uint32_t ioRange = 0;

static int hello_proc_show(struct seq_file *m, void *v) {
  seq_printf(m, "Hello procdriver!\n");
  return 0;
}

static int hello_proc_open(struct inode *inode, struct  file *file) {
  return single_open(file, hello_proc_show, NULL);
}
static int hello_proc_write(struct file *file, const char *buffer, unsigned long count,void *data)
{
	char input[1];
	printk(KERN_DEBUG MODULE_NAME "%s :%d",buffer,count);
	if(count > 1)
	{
		copy_from_user(input,buffer,1);
		if(input[0] == '1')
		{
			printk(KERN_DEBUG MODULE_NAME "1 Written\n");
			int setIndex = GPIO/32;
                        uint32_t writeVal =  ( 1<< (GPIO %32));
                        iounmap(iomap);
                        iomap = ioremap(ioRange ^ 28, 32);
                        iowrite32(writeVal, iomap);
			iounmap(iomap);
		}
		else if (input[0] == '0')
		{
			printk(KERN_DEBUG MODULE_NAME "0 Written\n");
			int setIndex = GPIO/32;
                        uint32_t writeVal =  ( 1<< (GPIO %32));
                        iounmap(iomap);
                        iomap = ioremap(ioRange ^ 40 , 32);
                        iowrite32(writeVal, iomap);
			iounmap(iomap);
		}
		else
		{
			printk(KERN_DEBUG MODULE_NAME "Wrong Input:%s\n",input);
		}
	}
	else
	{
		printk(KERN_DEBUG MODULE_NAME "Input Too Long\n");
	}
	return 1;
}

static const struct file_operations hello_proc_fops = {
  .owner = THIS_MODULE,
  .open = hello_proc_open,
  .write = hello_proc_write,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

static int __init hello_proc_init(void) {

	proc_create("procdriver",0, NULL, &hello_proc_fops);
	if(procFile == NULL)
	{
		printk(KERN_DEBUG MODULE_NAME "ERROR on creating File\n");
	}
	struct device_node *node = NULL;
	int ret;
	uint32_t ranges[] ={0,0,0};
  	printk(KERN_DEBUG MODULE_NAME "init procdriver!\n");
	node = of_find_node_by_name(node, "soc");
	if(node != NULL)
	{
		ret = of_property_read_u32_array(node, "ranges", ranges, ARRAY_SIZE(ranges));
		if( ret == 0);
		{

			printk(KERN_DEBUG MODULE_NAME "0: %x\n",ranges[1]); 
			ioRange = ranges[1]^ 2097152;
			printk(KERN_DEBUG MODULE_NAME "io startAddress %x\n", ioRange);
			int registerIndex = GPIO /10;
			int bit = (GPIO %10) * 3;
			uint32_t mask = 0b111 << bit;
			iomap = ioremap(ioRange ^ (4*registerIndex),32);
			uint32_t oldVal = ioread32(iomap);
			printk(KERN_DEBUG MODULE_NAME "old %x\n", oldVal);
			uint32_t newVal = (oldVal &~ mask) | ((0b001 << bit) & mask);
			printk(KERN_DEBUG MODULE_NAME "new %x\n", newVal);
			iowrite32(newVal, iomap);
			int setIndex = GPIO/32;
			uint32_t writeVal =  ( 1<< (GPIO %32));
			iounmap(iomap);
			iomap = ioremap(ioRange ^ 28, 32);
			iowrite32(writeVal, iomap);


		}
	}
  	return 0;
}

static void __exit hello_proc_exit(void) {
	iounmap(iomap);
  	remove_proc_entry("procdriver", NULL);
  	printk(KERN_DEBUG MODULE_NAME "exit procdriver!\n");
}

MODULE_LICENSE("GPL");
module_init(hello_proc_init);
module_exit(hello_proc_exit);
