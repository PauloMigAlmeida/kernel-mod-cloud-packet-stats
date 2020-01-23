/*
 * packetcounter.h
 *
 *  Created on: 19/01/2020
 *      Author: paulo
 */

#ifndef KERNEL_CLOUD_PACKET_COUNTER_H_
#define KERNEL_CLOUD_PACKET_COUNTER_H_

#include <linux/errno.h>
#include <linux/stddef.h>
#include <linux/string.h>

#define AWS_CLOUD_INTERNAL aws
#define GCP_CLOUD_INTERNAL gcp
#define AZURE_CLOUD_INTERNAL azure
#define AWS_CLOUD __stringify(AWS_CLOUD_INTERNAL)
#define GCP_CLOUD __stringify(GCP_CLOUD_INTERNAL)
#define AZURE_CLOUD __stringify(AZURE_CLOUD_INTERNAL)

void init_counters(void);
int reset_counter(const char* cloud_provider_name);
long long int get_counter(const char* cloud_provider_name);
void increment_counter(const char* cloud_provider_name);

#endif /* KERNEL_CLOUD_PACKET_COUNTER_H_ */
