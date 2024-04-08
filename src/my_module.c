// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2024 Aleksandra Smolniakova.
 */

#include <linux/module.h>

// Module metadata
MODULE_AUTHOR("Aleksandra Smolniakova");
MODULE_DESCRIPTION("Hello KernelCare driver");
MODULE_LICENSE("GPL");

// Custom init and exit methods
static int __init custom_init(void)
{
	pr_debug("Hello KernelCare!");
	return 0;
}

static void __exit custom_exit(void)
{
	pr_debug("Hello KernelCare is unloaded");
}
module_init(custom_init);
module_exit(custom_exit);
