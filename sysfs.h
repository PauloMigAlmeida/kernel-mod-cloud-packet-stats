// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * sysfs.h
 *
 *  Created on: 19/01/2020
 *      Author: Paulo Miguel Almeida
 */
#ifndef KERNEL_CLOUD_PACKET_STATS_SYSFS_H
#define KERNEL_CLOUD_PACKET_STATS_SYSFS_H

#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/errno.h>
#include "packetcounter.h"

int sysfs_init(void);
void sysfs_destroy(void);

#endif //KERNEL_CLOUD_PACKET_STATS_SYSFS_H
