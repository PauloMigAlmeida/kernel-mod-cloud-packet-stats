// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * cidr.h
 *
 *  Created on: 23/01/2020
 *      Author: Paulo Miguel Almeida
 */

#ifndef KERNEL_CLOUD_CIDR_H_
#define KERNEL_CLOUD_CIDR_H_

#include <linux/bsearch.h>
#include <linux/types.h>
#include "packetcounter.h"

void process_ip(u32 saddr);

#endif /* KERNEL_CLOUD_CIDR_H_ */
