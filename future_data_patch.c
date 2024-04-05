// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2024 Aleksandra Smolniakova.
 */
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
#include <linux/mutex.h>
#include <linux/stat.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 11

// Module metadata
MODULE_AUTHOR("Aleksandra Smolniakova");
MODULE_DESCRIPTION("Hello KernelCare driver data patch");
MODULE_LICENSE("GPL");


// This directory entry will point to `/sys/kernel/debug/kernelcare`.
static struct dentry *dir = 0;


static struct mutex foo_mutex;
static char foo_tmp[PAGE_SIZE];
static int foo_size;

// static long int jiffies_read_op(struct file *fp, char *buff, size_t count,
// 								loff_t *off) {
// 	// printk("READ JIFF!\n");
// 	// char *my_str = "My str\n";
// 	// // snprintf(my_str, 11, "%ld\n", jiffies);
// 	// // if (copy_to_user(buff, my_str, sizeof(my_str))) 
// 	// // 	return -EFAULT;
// 	// printk(my_str);
// 	// return sizeof(my_str);
// 	return 1;
// }

static long int jiffies_read_op(struct file *fp, char *buff, size_t count,
								loff_t *off) {
	long j = jiffies;
	char my_str[BUFFER_SIZE + 1]; // +1 for null terminator
	int len;
	len = snprintf(my_str, BUFFER_SIZE + 1, "%ld", j);
	return simple_read_from_buffer(buff, count, off, my_str, 11);
}

static ssize_t data_read_op(struct file *fp, char *buff, size_t count,
								loff_t *off)
{
	// printk("HERE!\n");
	int ret;
	// ret = mutex_lock(&foo_mutex);
	// if (ret)
	// 	return ret;
	mutex_lock(&foo_mutex);
	if (count > foo_size - *off)
		count = foo_size - *off;

	if (!count) {
		ret = 0;
		goto cleanup;
	}

	if (copy_to_user(buff, foo_tmp + *off, count)) {
		ret = -EFAULT;
		goto cleanup;
	}
	// printk("READ!\n");
	*off += count;
	ret = count;

	cleanup:
		mutex_unlock(&foo_mutex);
		return ret;
}

static ssize_t data_write_op(struct file *fp, const char *buff, size_t count,
								loff_t *off)
{
	int ret;
	// ret = mutex_lock(&foo_mutex);
	// if (ret)
	// 	return ret;
	mutex_lock(&foo_mutex);
	count = min(count, PAGE_SIZE);

	if (copy_from_user(foo_tmp, buff, count)) {
		ret = -EFAULT;
		goto cleanup;
	}
	printk("WRITEN!\n");
	printk(buff);
	foo_size = count;
	ret = count;

	cleanup:
		mutex_unlock(&foo_mutex);
		return ret;	
}

struct file_operations fops_j = {
       .read = jiffies_read_op,
        .write = NULL,
        .open = NULL
    };

struct file_operations fops_d = {
	.read = data_read_op,
	.write = data_write_op
    };

// Custom init and exit methods
static int __init custom_init(void) {
  struct dentry *file1;
  struct dentry *file2;

  foo_size = 0;

   mutex_init(&foo_mutex);

  dir = debugfs_create_dir("kernelcare", 0);
  if (!dir) {
    // Abort module load.
    pr_alert("debugfs_kernelcare: failed to create /sys/kernel/debug/kernelcare\n");
    return -1;
  }

  file1 = debugfs_create_file("jiffies", 0004, dir, NULL, &fops_j);

  if (!file1) {
    // Abort module load.
    pr_alert("debugfs_kernelcare: failed to create "
           "/sys/kernel/debug/kernelcare/jiffies\n");
    return -1;
  }

    file2 = debugfs_create_file("data", 0204, dir, NULL, &fops_d);
   

  if (!file2) {
    // Abort module load.
    pr_alert("debugfs_kernelcare: failed to create "
           "/sys/kernel/debug/kernelcare/data\n");
    return -1;
  }

  pr_debug("Hello KernelCare!");
  return 0;
}

static void __exit custom_exit(void) {
  pr_debug("Hello KernelCare is unloaded");
  debugfs_remove_recursive(dir);
  
}

module_init(custom_init);
module_exit(custom_exit);
