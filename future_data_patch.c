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

// #define PAGE_SIZE 4096

// Module metadata
MODULE_AUTHOR("Aleksandra Smolniakova");
MODULE_DESCRIPTION("Hello KernelCare driver data patch");
MODULE_LICENSE("GPL");


// This directory entry will point to `/sys/kernel/debug/kernelcare`.
static struct dentry *dir = 0;

// static u32 data = 0;

// static struct file *data_ptr = NULL;

static struct mutex foo_mutex;
static char foo_tmp[PAGE_SIZE];
static int foo_size;

// static short  messageSize;
// static char   message[256] = {0};

static long int jiffies_read_op(struct file *, char *, long unsigned int,  long long int *) {
	long j = jiffies;
  printk("\n[Jiffies Time : %lu]", j);
  return j;
}

// static int data_release(struct inode *inode, struct file *file)
// {
//     mutex_unlock(&foo_mutex);
//     return 0;
// }

// static int data_open(struct inode *inode, struct file *file)
// {
//     if (!mutex_trylock(&foo_mutex)) {
//         printk(KERN_ALERT "data is in use, but will still go on.");
//         // FIXME: If we want the mutex lock to work well, need to return -EBUSY
//        return -EBUSY;
//     }
//     return 0;
// }

static ssize_t data_read_op(struct file *fp, char *buff, size_t count,
								loff_t *off)
{
	printk("HERE!\n");
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
	printk("READ!\n");
	*off += count;
	ret = count;

	cleanup:
		mutex_unlock(&foo_mutex);
		return ret;
}

// static ssize_t data_read_op(struct file *filep, char *buffer, size_t len, loff_t *offset){
//    int error_count = 0;
// //    int i = 0;
//    char babelMessage[256] = { 0 };

//    //   Create the babel content.
// //    for (; i<messageSize; i++) {
// //        babelMessage[i] = babel(message[i]);
// //    }

//    // copy_to_user has the format ( * to, *from, size) and returns 0 on success
//    error_count = copy_to_user(buffer, babelMessage, messageSize);

//    if (error_count==0) {
// //       pr_info("%s: sent %d characters to the user\n", MODULE_NAME, messageSize);
//       //    Clear the position, return 0.
//       messageSize = 0;
//       return 0;
//    }
//    else {
// //       pr_err("%s: failed to send %d characters to the user\n", MODULE_NAME, error_count);
//       //    Failed -- return a bad address message (i.e. -14)
//       return -EFAULT;              
//    }
// }

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

// static ssize_t data_write_op(struct file *filep, const char *buffer, size_t len, loff_t *offset){
//     //  Write the string into our message memory. Record the length.
//     long copied_from_user;
//     copied_from_user = copy_from_user(message, buffer, len);
//     messageSize = len;
//     if(copied_from_user != 0 )
//     {
//         printk(KERN_WARNING "copy_from_user failed ret_val: %ld\n",copied_from_user );
//     }
//     else
//     {
//         // pr_info("%s: copy_from_user ret_val : %ld\n", MODULE_NAME, copied_from_user);
//         // pr_info("%s: received %zu characters from the user\n", MODULE_NAME, len);
//     }

//     return len;
// }

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
//   mutex_init(&foo_mutex);
  return 0;
}

static void __exit custom_exit(void) {
  pr_debug("Hello KernelCare is unloaded");
//   if(data_ptr)
//   	kfree(data_ptr);
  debugfs_remove_recursive(dir);
  
}

module_init(custom_init);
module_exit(custom_exit);
