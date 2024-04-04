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

// Module metadata
MODULE_AUTHOR("Aleksandra Smolniakova");
MODULE_DESCRIPTION("Hello KernelCare driver jiffies patch");
MODULE_LICENSE("GPL");

// This directory entry will point to `/sys/kernel/debug/kernelcare`.
static struct dentry *dir = 0;

static long int jiffies_read_op(struct file *, char *, long unsigned int,  long long int *) {
	long j = jiffies;
  printk("\n[Jiffies Time : %lu]", j);
  return j;
}

struct file_operations fops = {
       .read = jiffies_read_op,
        .write = NULL,
        .open = NULL
    };

// Custom init and exit methods
static int __init custom_init(void) {
  struct dentry *file1;

  dir = debugfs_create_dir("kernelcare", 0);
  if (!dir) {
    // Abort module load.
    pr_alert("debugfs_kernelcare: failed to create /sys/kernel/debug/kernelcare\n");
    return -1;
  }

  file1 = debugfs_create_file("jiffies", 0004, dir, NULL, &fops);

 if (!file1) {
    // Abort module load.
    pr_alert("debugfs_kernelcare: failed to create "
           "/sys/kernel/debug/kernelcare/jiffies\n");
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
