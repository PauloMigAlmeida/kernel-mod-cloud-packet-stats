// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * sysfs.c
 *
 *  Created on: 19/01/2020
 *      Author: Paulo Miguel Almeida
 */

#include "sysfs.h"

//Macros
#define SYSFS_CREATE_FILE(kobject, kobj_attribute)                         \
	if (sysfs_create_file(kobject, &kobj_attribute.attr))              \
	{															\
		pr_err("failed to create /sys/kernel/%s/%s",               \
				kobject->name, kobj_attribute.attr.name);  \
		return -ENOMEM;											\
	}

//Prototypes
static ssize_t packet_counter_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf);
static ssize_t packet_counter_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count);

static struct kobject *cloud_packet_stats;
static struct kobj_attribute aws_packet_counter_attr =
				__ATTR(AWS_CLOUD_INTERNAL, S_IRUGO | S_IWUSR, packet_counter_show, packet_counter_store);
static struct kobj_attribute gcp_packet_counter_attr =
				__ATTR(GCP_CLOUD_INTERNAL, S_IRUGO | S_IWUSR, packet_counter_show, packet_counter_store);
static struct kobj_attribute azure_packet_counter_attr =
				__ATTR(AZURE_CLOUD_INTERNAL, S_IRUGO | S_IWUSR, packet_counter_show, packet_counter_store);

static ssize_t packet_counter_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	long long int result;
	long long int counter = get_counter(attr->attr.name);
	if (counter < 0)
		result = counter; //likely -EINVAL
	else
		result = sprintf(buf, "%lli\n", get_counter(attr->attr.name));

	return result;
}

static ssize_t packet_counter_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	int result;

	if (!strcmp(buf, "reset\n"))
		result = reset_counter(attr->attr.name);
	else
		result = -EINVAL;

	return result ? result : count;
}

void sysfs_destroy(void)
{
	if (cloud_packet_stats)
		kobject_del(cloud_packet_stats);
}

int sysfs_init(void)
{
	cloud_packet_stats = kobject_create_and_add("cloud_packet_stats", NULL);
	if (!cloud_packet_stats) {
		pr_info(
				"%s: error creating kobject with name 'cloud_packet_stats'",
				KBUILD_MODNAME);
		return -ENOMEM;
	}

	SYSFS_CREATE_FILE(cloud_packet_stats, aws_packet_counter_attr);
	SYSFS_CREATE_FILE(cloud_packet_stats, gcp_packet_counter_attr);
	SYSFS_CREATE_FILE(cloud_packet_stats, azure_packet_counter_attr);

	init_counters();

	return 0;
}
