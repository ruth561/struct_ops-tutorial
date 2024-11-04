// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_AUTHOR("Takumi Jin");
MODULE_DESCRIPTION("A simple implementation of a kernel module using struct_ops");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("0.1");

static int __init my_ops_init(void)
{
	pr_info("my_ops_init\n");
	return 0;
}

static void __exit my_ops_exit(void)
{
	pr_info("my_ops_exit\n");
}

module_init(my_ops_init);
module_exit(my_ops_exit);
