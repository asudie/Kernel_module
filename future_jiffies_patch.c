#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

// Module metadata
MODULE_AUTHOR("Aleksandra Smolniakova");
MODULE_DESCRIPTION("Hello KernelCare driver");
MODULE_LICENSE("GPL");

// This directory entry will point to `/sys/kernel/debug/kernelcare`.
static struct dentry *dir = 0;

// File `/sys/kernel/debug/kernelcare/jiffies` points to this variable.
static u32 jiffies = 0;
static u32 timer = 0;

static int jiffies_read_op() {
  timer = 5;
  // TO DO: normal timer
  return timer;
}

//
// The macro has form
//     DEFINE_SIMPLE_ATTRIBUTE(fops_name, read_op, write_op, printf_fmt)
// and it defines `add_ops` to be
//     struct file_operations add_ops = {
//         .read = NULL,
//         .write = add_write_op,
//         .open = add_fops_open, // Also defined by macro
//         // ...
//     };
//

DEFINE_SIMPLE_ATTRIBUTE(jiffies_fops, jiffies_read_op, NULL, "%llu\n");

// Custom init and exit methods
static int __init custom_init(void) {
  struct dentry *file1;

  dir = debugfs_create_dir("kernelcare", 0);
  if (!dir) {
    // Abort module load.
    printk(
        KERN_ALERT
        "debugfs_kernelcare: failed to create /sys/kernel/debug/kernelcare\n");
    return -1;
  }

  file1 = debugfs_create_u32("jiffies", 0004, dir, &jiffies);

  // TO DO: when read, should return the current value of the jiffies kernel
  // timer

  if (!file1) {
    // Abort module load.
    printk(KERN_ALERT
           "debugfs_kernelcare: failed to create "
           "/sys/kernel/debug/kernelcare/jiffies\n");
    return -1;
  }

  printk(KERN_DEBUG "Hello KernelCare!");
  return 0;
}

static void __exit custom_exit(void) {
  printk(KERN_DEBUG "Hello KernelCare is unloaded");
}

void cleanup_module(void) {
  // We must manually remove the debugfs entries we created. They are not
  // automatically removed upon module removal.
  debugfs_remove_recursive(dir);
}

module_init(custom_init);
module_exit(custom_exit);
