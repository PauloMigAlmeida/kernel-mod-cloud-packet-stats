// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * packetcounter.c
 *
 *  Created on: 19/01/2020
 *      Author: Paulo Miguel Almeida
 */

#include "packetcounter.h"

/*
 * Rationale:
 * Depending on the number of physical network interfaces you have on
 * your machine, you can have interrupts handled in different cores.
 * This alone forces us to have some sort of locking mechanism if we
 * want to have a centralised counter. Another mechanism is to define
 * variables per CPU and sum them up when we need to read it which exempt
 * us from dealing with a good porting of SMP problems. (which is what
 * I decided to do)
 */
DEFINE_PER_CPU(unsigned long int, aws_packet_counter);
DEFINE_PER_CPU(unsigned long int, gcp_packet_counter);
DEFINE_PER_CPU(unsigned long int, azure_packet_counter);

unsigned long long int read_counter(unsigned long int* ref)
{
	unsigned long long int sum = 0;
	unsigned int cpu;

	preempt_disable();
	for_each_online_cpu(cpu)
	{
		int total_per_cpu = *per_cpu_ptr(ref, cpu);
		sum += total_per_cpu;
//		pr_info("%s: cpu:%u total_per_cpu: %u", KBUILD_MODNAME, cpu, total_per_cpu);
	}
	preempt_enable();

	return sum;
}

void write_counter(unsigned long int* ref, unsigned long int value)
{
	unsigned int cpu;

	preempt_disable();
	for_each_online_cpu(cpu)
	{
		*per_cpu_ptr(ref, cpu) = value;
	}
	preempt_enable();
}

__always_inline unsigned long int* find_counter_ref(const char *name)
{
	if (!strcmp(name, AWS_CLOUD))
		return &aws_packet_counter;
	else if (!strcmp(name, GCP_CLOUD))
		return &gcp_packet_counter;
	else if (!strcmp(name, AZURE_CLOUD))
		return &azure_packet_counter;
	else
		return NULL;
}

void init_counters(void)
{
	write_counter(&aws_packet_counter, 0);
	write_counter(&gcp_packet_counter, 0);
	write_counter(&azure_packet_counter, 0);
}

int reset_counter(const char *cloud_provider_name)
{
	int result = 0;
	unsigned long int *ref = find_counter_ref(cloud_provider_name);

	if (ref)
		write_counter(ref, 0);
	else
		result = -EINVAL;

	return result;
}

unsigned long long int get_counter(const char *cloud_provider_name)
{
	unsigned long long int result;
	unsigned long int *ref = find_counter_ref(cloud_provider_name);

	if (ref)
		result = read_counter(ref);
	else
		result = -EINVAL;

	return result;
}

void increment_counter(const char *cloud_provider_name)
{
	unsigned long int *ref = find_counter_ref(cloud_provider_name);
	if (ref)
		this_cpu_inc(*ref); //This should take care of preemptable stuff
	/*
	 * I don't like this approach very much...I will try to refactor
	 * find_counter_ref to avoid "&*ptr"ing it.
	 */
}
