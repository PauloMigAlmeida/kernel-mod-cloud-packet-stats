/*
 * netfilter.h
 *
 *  Created on: 19/01/2020
 *      Author: paulo
 */

#ifndef KERNEL_CLOUD_NETFILTER_H_

#define KERNEL_CLOUD_NETFILTER_H_

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include "cidr.h"

int cloud_packet_netfilter_init(void);
void cloud_packet_netfilter_destroy(void);

#endif /* KERNEL_CLOUD_NETFILTER_H_ */
