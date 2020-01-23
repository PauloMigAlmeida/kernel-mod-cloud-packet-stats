/*
 * module.c
 *
 *  Created on: 19/01/2020
 *      Author: paulo
 */
#include <linux/module.h>
#include <linux/init.h>
#include "sysfs.h"
#include "netfilter.h"

static int __init m_init(void)
{
	int err_ret;

	err_ret = sysfs_init();
	if(err_ret){
		pr_err("%s: error while executing sysfs_init()\n", KBUILD_MODNAME);
		return err_ret;
	}

	err_ret = cloud_packet_netfilter_init();
	if(err_ret){
		pr_err("%s: error while executing netfilter_init()\n", KBUILD_MODNAME);
		return err_ret;
	}

	pr_info("%s: module loaded from 0x%p\n", KBUILD_MODNAME, m_init);
	return 0;
}

static void __exit m_exit(void)
{
	pr_info("%s: module unloaded from 0x%p\n", KBUILD_MODNAME, m_exit);
	sysfs_destroy();
	cloud_packet_netfilter_destroy();
}

module_init(m_init);
module_exit(m_exit);

MODULE_AUTHOR("Paulo Miguel Almeida");
MODULE_LICENSE("GPL v2");
