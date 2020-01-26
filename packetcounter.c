// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * packetcounter.c
 *
 *  Created on: 19/01/2020
 *      Author: Paulo Miguel Almeida
 */

#include "packetcounter.h"

static long long int aws_packet_counter;
static long long int gcp_packet_counter;
static long long int azure_packet_counter;

long long int* find_counter_ref(const char *name)
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
	aws_packet_counter = 0;
	gcp_packet_counter = 0;
	azure_packet_counter = 0;
}

int reset_counter(const char *cloud_provider_name)
{
	int result = 0;
	long long int *ref = find_counter_ref(cloud_provider_name);

	if (ref)
		*ref = 0;
	else
		result = -EINVAL;

	return result;
}

long long int get_counter(const char *cloud_provider_name)
{
	long long int result;
	long long int *ref = find_counter_ref(cloud_provider_name);

	if (ref)
		result = *ref;
	else
		result = -EINVAL;

	return result;
}

void increment_counter(const char *cloud_provider_name)
{
	long long int *ref = find_counter_ref(cloud_provider_name);
	//FIXME: Make sure that this operation happens atomically.
	if (ref)
		*ref += 1;
}
