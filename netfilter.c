// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * netfilter.c
 *
 *  Created on: 19/01/2020
 *      Author: Paulo Miguel Almeida
 */
#include "netfilter.h"

static struct nf_hook_ops nfho;

static unsigned int cloud_packet_tcp_packet_filter(const struct nf_hook_ops *ops,
		struct sk_buff *skb, const struct net_device *in,
		const struct net_device *out, int (*okfn)(struct sk_buff*)) {
	struct iphdr *iph; 			/* IPv4 header */
	u32 saddr; 					/* Source address */

	if (!skb)					/* Network packet is empty */
		return NF_ACCEPT;

	iph = ip_hdr(skb); 			/* get IP header */
	saddr = ntohl(iph->saddr);
	process_ip(saddr);

	return NF_ACCEPT;
}

int cloud_packet_netfilter_init(void) {
	int res = 0;
	struct net *n;

	nfho.hook = (nf_hookfn*) cloud_packet_tcp_packet_filter; 	/* hook function */
	nfho.hooknum = NF_INET_PRE_ROUTING; 						/* received packets */
	nfho.pf = PF_INET; 											/* IPv4 */
	nfho.priority = NF_IP_PRI_FIRST; 							/* max hook priority */

	for_each_net(n){
		res = nf_register_net_hook(n, &nfho);
		if (res)
			return res;
	}

	return res;
}

void cloud_packet_netfilter_destroy(void) {
    struct net *n;
    for_each_net(n)
        nf_unregister_net_hook(n, &nfho);
}
