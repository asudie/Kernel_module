#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

// Module metadata
MODULE_AUTHOR("Aleksandra Smolniakova");
MODULE_DESCRIPTION("Hello KernelCare driver");
MODULE_LICENSE("GPL");

// Custom init and exit methods
static int __init custom_init(void) {
 printk(KERN_DEBUG "Hello KernelCare!");
 return 0;
}
static void __exit custom_exit(void) {
 printk(KERN_DEBUG "Hello KernelCare is unloaded");
}
module_init(custom_init);
module_exit(custom_exit);
