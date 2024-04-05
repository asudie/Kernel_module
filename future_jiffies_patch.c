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

#define BUFFER_SIZE 11

// Module metadata
MODULE_AUTHOR("Aleksandra Smolniakova");
MODULE_DESCRIPTION("Hello KernelCare driver jiffies patch");
MODULE_LICENSE("GPL");

// This directory entry will point to `/sys/kernel/debug/kernelcare`.
static struct dentry *dir;

static long jiffies_read_op(struct file *fp, char *buff, size_t count, loff_t *off)
{
	long j = jiffies;
	char my_str[BUFFER_SIZE + 1]; // +1 for null terminator
	int len;

	len = snprintf(my_str, BUFFER_SIZE + 1, "%ld", j);
	// printk("JIFFIE READS!");
	return simple_read_from_buffer(buff, count, off, my_str, 11);
}

const struct file_operations fops_j = {
	.read = jiffies_read_op,
	.write = NULL,
	.open = NULL
};

// Custom init and exit methods
static int __init custom_init(void)
{
	struct dentry *file1;

	dir = 0;
	dir = debugfs_create_dir("kernelcare", 0);
	if (!dir) {
		// Abort module load.
		pr_alert("debugfs_kernelcare: failed to create /sys/kernel/debug/kernelcare\n");
		return -1;
	}

	file1 = debugfs_create_file("jiffies", 0004, dir, NULL, &fops_j);

	if (!file1) {
		// Abort module load.
		pr_alert("debugfs_kernelcare: failed to create /sys/kernel/debug/kernelcare/jiffies\n");
		return -1;
	}

	 pr_debug("Hello KernelCare!");
	return 0;
}

static void __exit custom_exit(void)
{
	pr_debug("Hello KernelCare is unloaded");
	debugfs_remove_recursive(dir);
}

module_init(custom_init);
module_exit(custom_exit);
