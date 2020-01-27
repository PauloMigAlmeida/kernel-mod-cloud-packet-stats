// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * netfilter.c
 *
 *  Created on: 19/01/2020
 *      Author: Paulo Miguel Almeida
 */
#include "netfilter.h"

//prototypes
int __net_init cloud_packet_netdevice_init(struct net *net);
void __net_exit cloud_packet_netdevice_exit(struct net *net);

static struct nf_hook_ops nfho;
static struct pernet_operations pnop = {
		.init = cloud_packet_netdevice_init,
		.exit = cloud_packet_netdevice_exit,
};

//nf_hook_ops callback method
static unsigned int cloud_packet_tcp_packet_filter(void *priv,
		struct sk_buff *skb,
		const struct nf_hook_state *state)
{
	struct iphdr *iph; /* IPv4 header */
	u32 saddr; /* Source address */

	if (!skb) /* Network packet is empty */
		return NF_ACCEPT;

	iph = ip_hdr(skb); /* get IP header */
	saddr = ntohl(iph->saddr);
	process_ip(saddr);

	return NF_ACCEPT;
}

//pernet_operations callback methods
int __net_init cloud_packet_netdevice_init(struct net *net)
{
	int res = nf_register_net_hook(net, &nfho);
	/*
	 * Rationale: in very specific occasions nf_register_net_hook
	 * can return something different from success. The fact is
	 * that when nf_register_net_hook fails, it doesn't mean that
	 * pernet_operations.init failed too. Also, nf_unregister_net_hook
	 * seems to be pretty smart when handling not registered devices.
	 *
	 * In case someone finds a better way of handling these exceptional
	 * cases, please submit a PR. I would love to learn a bit more about
	 * this.
	 */
	if (res)
		pr_err("%s: couldn't register_net_hook for new ns_dev",
				KBUILD_MODNAME);

	return 0; //success
}
void __net_exit cloud_packet_netdevice_exit(struct net *net)
{
	nf_unregister_net_hook(net, &nfho);
}

int cloud_packet_netfilter_init(void)
{
	int res;
	struct net *n;

	nfho.hook = cloud_packet_tcp_packet_filter; /* hook function */
	nfho.hooknum = NF_INET_PRE_ROUTING; /* received packets */
	nfho.pf = PF_INET; /* IPv4 */
	nfho.priority = NF_IP_PRI_FIRST; /* max hook priority */

	/*
	 * register listener to newly created/removed interfaces,
	 * regardless of their network namespace.
	 */
	res = register_pernet_device(&pnop);
	if (res) {
		pr_err("%s: error while executing register_pernet_device()\n",
				KBUILD_MODNAME);
		goto fail_reg_pernet_dev;
	}

	//hook into every available interface
	for_each_net(n)
	{
		res = nf_register_net_hook(n, &nfho);
		if (res) {
			pr_err("%s: error while registering net hook\n",
					KBUILD_MODNAME);
			goto fail_reg_net_hook;
		}
	}

	return 0;

	fail_reg_net_hook:
	for_each_net(n)
		nf_unregister_net_hook(n, &nfho);
	fail_reg_pernet_dev:
	unregister_pernet_device(&pnop);
	return res;

}

void cloud_packet_netfilter_destroy(void)
{
	struct net *n;

	unregister_pernet_device(&pnop);
	for_each_net(n)
		nf_unregister_net_hook(n, &nfho);
}
