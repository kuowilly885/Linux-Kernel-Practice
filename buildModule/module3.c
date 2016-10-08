#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>

MODULE_AUTHOR("Rongshian Kuo");

static char *myname = "Default";
static char *mystreet = "Default";
static char *mycity = "Default";
static int zipcode = 0;

module_param(myname, charp, 0000);
MODULE_PARM_DESC(myname, "A character string");
module_param(mystreet, charp, 0000);
MODULE_PARM_DESC(mystreet, "A character string");
module_param(mycity, charp, 0000);
MODULE_PARM_DESC(mycity, "A character string");
module_param(zipcode, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(zipcode, "An integer");

static int __init hello_5_init(void)
{
	printk(KERN_INFO "Loading Hello 3: Hello World 3\n");
	printk(KERN_INFO "==============================\n");
	printk(KERN_INFO "Name : %s\n", myname);
	printk(KERN_INFO "Street : %s\n", mystreet);
	printk(KERN_INFO "City : %s\n", mycity);
	printk(KERN_INFO "ZIP code : %5d\n", zipcode);
	return 0;
}

static void __exit hello_5_exit(void)
{
	printk(KERN_INFO "Goodbye, world 3\n");
}

module_init(hello_5_init);
module_exit(hello_5_exit);
