#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static char init[] __initdata = "Rongshian Kuo: Loading Hello2 module - Hello World 2";

static int __init hello_2_init(void)
{
	printk(KERN_INFO "%s\n", init);
	return 0;
}

static void __exit hello_2_exit(void)
{
	char exit[] = "Rongshian Kuo: Exiting Hello2 module - Goodbye World 2";
	printk(KERN_INFO "%s\n", exit);
}

module_init(hello_2_init);
module_exit(hello_2_exit);
