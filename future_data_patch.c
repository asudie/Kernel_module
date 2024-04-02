#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <stdio.h>
// Module metadata
MODULE_AUTHOR("Aleksandra Smolniakova");
MODULE_DESCRIPTION("Hello KernelCare driver data patch");
MODULE_LICENSE("GPL");

// This directory entry will point to `/sys/kernel/debug/kernelcare`.
static struct dentry *dir = 0;

static u32 data = 0;

static struct file *data_ptr;

static long int jiffies_read_op(struct file *, char *, long unsigned int,  long long int *) {
	long j = jiffies;
  printk("\n[Jiffies Time : %lu]", j);
  return j;
}

static long int data_read_op(struct file *, char *, long unsigned int,  long long int *) {
	long j = jiffies;
  printk("\n[Jiffies Time : %lu]", j); // TO DO
  return j;
}

static char *data_write_op(struct file *fptr, const char __user *data, size_t size, loff_t *mem)
{
	struct FILE *my_ptr;

	data_ptr = kmalloc(sizeof(struct file), GFP_KERNEL);
	if (!fptr)
		printk(
        KERN_ALERT
        "Can't allocate memory!\n");

	my_ptr = fopen("/sys/kernel/debug/kernelcare/data","w");
	 if(my_ptr == NULL)
	{
	printk(
        KERN_ALERT
        "Can't open the file!\n");  
	exit(1);             
	}

	printf(data);
	fclose(my_ptr);

   return data; // TO DO: return data
}


struct file_operations fops_j = {
       .read = jiffies_read_op,
        .write = NULL,
        .open = NULL
    };

struct file_operations fops_d = {
       .read = data_read_op,
        .write = data_write_op,
        .open = NULL
    };

// Custom init and exit methods
static int __init custom_init(void) {
  struct dentry *file1;
  struct dentry *file2;

  dir = debugfs_create_dir("kernelcare", 0);
  if (!dir) {
    // Abort module load.
    printk(
        KERN_ALERT
        "debugfs_kernelcare: failed to create /sys/kernel/debug/kernelcare\n");
    return -1;
  }

  file1 = debugfs_create_file("jiffies", 0004, dir, NULL, &fops_j);

  if (!file1) {
    // Abort module load.
    printk(KERN_ALERT
           "debugfs_kernelcare: failed to create "
           "/sys/kernel/debug/kernelcare/jiffies\n");
    return -1;
  }

    file2 = debugfs_create_file("data", 0204, dir, NULL, &fops_d);
  /* TO DO: when writing to it, the value must be stored, up to one page
of data. When read, which can be done by any user, the value that is
stored in it must be returned. Properly handle the fact that someone
could be reading from the file while someone else is writing to it
(oh, a locking hint!)*/
  if (!file2) {
    // Abort module load.
    printk(KERN_ALERT
           "debugfs_kernelcare: failed to create "
           "/sys/kernel/debug/kernelcare/data\n");
    return -1;
  }

  printk(KERN_DEBUG "Hello KernelCare!\n");
  return 0;
}

static void __exit custom_exit(void) {
  printk(KERN_DEBUG "Hello KernelCare is unloaded\n");
  kfree(data_ptr);
  debugfs_remove_recursive(dir);
  
}

module_init(custom_init);
module_exit(custom_exit);
