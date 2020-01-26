// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * cidr.c
 *
 *  Created on: 23/01/2020
 *      Author: Paulo Miguel Almeida
 */

#include "cidr.h"

#ifdef __CDT_PARSER__
/*
 * Eclipse CDT parser can become extremely annoying sometimes.
 * These variables will be created during the building phase of
 * the module.
 */
static u32 aws_cidr_blocks[][] = {};
static u32 azure_cidr_blocks[][] = {};
static u32 gcp_cidr_blocks[][] = {};
#define aws_cidr_arr_rows 0
#define aws_cidr_arr_cols 0
#define azure_cidr_arr_rows 0
#define azure_cidr_arr_cols 0
#define gcp_cidr_arr_rows 0
#define gcp_cidr_arr_cols 0
#else
#include "cloud_cidr_block.in"
#endif

static int bsearch_compare_f(const void *key, const void *elt)
{
	const u32 *saddr = key;
	const u32 *addr_vector = elt;
	if (*saddr > *(addr_vector + 1))
		return 1;
	else if (*saddr < *(addr_vector))
		return -1;
	else
		return 0;
}

static bool ip_in_range(u32 saddr, u32 *cloud_provider, int rows, int cols)
{
	u32 *ret = bsearch(&saddr, cloud_provider,
			rows,
			sizeof(u32) * cols,
			bsearch_compare_f);
	return ret != NULL;
}

void process_ip(u32 saddr)
{
	if (ip_in_range(saddr, &aws_cidr_blocks[0][0], aws_cidr_arr_rows,
			aws_cidr_arr_cols))
		increment_counter(AWS_CLOUD);
	else if (ip_in_range(saddr, &gcp_cidr_blocks[0][0], gcp_cidr_arr_rows,
			gcp_cidr_arr_cols))
		increment_counter(GCP_CLOUD);
	else if (ip_in_range(saddr, &azure_cidr_blocks[0][0],
			azure_cidr_arr_rows, azure_cidr_arr_cols))
		increment_counter(AZURE_CLOUD);
}
