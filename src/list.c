// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2024 Aleksandra Smolniakova.
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/kernel.h>

// Module metadata
MODULE_AUTHOR("Aleksandra Smolniakova");
MODULE_DESCRIPTION("Hello KernelCare driver and list");
MODULE_LICENSE("GPL");

struct identity {
	char name[20];
	int id;
	bool hired;
	struct list_head list; // they say only 3 fields???
};

static struct identity identity_list;

int identity_create(char *name, int id)
{
	struct identity *new;
	new = kmalloc(sizeof(*new), GFP_KERNEL);
	if (!new)
		return -ENOMEM;

	strncpy(new->name, name, 20);
	new->id = id;
	new->hired = false;
	list_add_tail(&new->list, &identity_list.list);

	return 0;
}

struct identity *identity_find(int id)
{
	struct list_head *ptr;
	struct identity *entry;

	list_for_each(ptr, &identity_list.list) {
		entry = list_entry(ptr, struct identity, list);
		if (entry->id == id)
			return entry;
	}

	return NULL;
}

void identity_destroy(int id)
{
	struct identity *entry;

	entry = identity_find(id);
	if (entry)
		list_del(&entry->list);
}

int identity_hire(int id)
{
	struct identity *entry;
	int res = 0;

	entry = identity_find(id);
	if (entry)
		entry->hired = true;
	else
		res = 1;
	return res;
}

// Custom init and exit methods
static int __init custom_init(void)
{
	pr_debug("Hello KernelCare!");

	struct identity *temp;
	identity_create("YOUR_NAME", 1);
	identity_create("YOUR_FRIEND_NAME", 2);
	// ...
	temp = identity_find(1);
	pr_debug("id 1 = %s\n", temp->name);
	identity_hire(1);
	temp = identity_find(10);
	if (temp == NULL)
	pr_debug("id 10 not found\n");
	identity_destroy(2);
	identity_destroy(1);
	return 0;
}

static void __exit custom_exit(void)
{
	pr_debug("Hello KernelCare is unloaded");
}
module_init(custom_init);
module_exit(custom_exit);
