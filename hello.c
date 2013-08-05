#include <linux/init.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <linux/moduleparam.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#undef __KERNEL__
#include <linux/netfilter_ipv4.h>
#define __KERNEL__

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mike");

static struct nf_hook_ops nfho;

//static char *drop_ip = "\x8C\x6D\x16\xFC";

/* This is the hook function itself */
unsigned int hook_func(unsigned int hooknum, struct sk_buff **skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *))
{
	//struct sk_buff *sb = *skb;
	struct iphdr *iph = NULL;
	struct tcphdr *tcph = NULL;

//	if ((*skb != NULL) && (skb->pkt_type == PACKET_HOST) && (skb->protocol == htons(ETH_P_IP)))
//	{
	//if((*skb)!=NULL)
	//{
	//{
		iph = ip_hdr((skb)); // access ip header.
		/*
		* yaple: Process only TCP segments.
		*/

		//printk("get protocol %i\n",iph->protocol);
		if ((iph->protocol == IPPROTO_TCP) )
		{
			printk("TCP get \n");
			tcph = (struct tcphdr *)(skb_network_header((skb)) + ip_hdrlen((skb))); // access tcp header.
			printk("INFO: Source IP Address: %u.\n",iph->saddr);
			printk("INFO: Destination IP Address: %u.\n",iph->daddr);
			printk("INFO: Source Port: %u.\n",tcph->source);
			printk("INFO: Destination Port: %u.\n",tcph->dest);
		}
//	}
	
	//if( sb->network_header.iph->saddr == *(unsigned int*)drop_ip )
	//{
	//	printk("dropped packet on %d.%d.%d.%d\n",*drop_ip,*(drop_ip+1),*(drop_ip+2),*(drop_ip+3));
	//	return NF_DROP;
	//}else	
	//printk("packet accepted!!\n");
	return NF_ACCEPT; // Tells the system to accept the packet, and process the next one.
}

/* Initialisation routine */
int init_module()
{
	/* Fill in our hook structure */
	nfho.hook     = hook_func;         /* Handler function */
	nfho.hooknum  = NF_IP_PRE_ROUTING; 
	nfho.pf       = PF_INET;
	nfho.priority = NF_IP_PRI_FIRST;   /* Make our function first */

        nf_register_hook(&nfho);
	printk("module loading!!!");
        return 0;
}
	    /* Cleanup routine */
void cleanup_module()
{
        nf_unregister_hook(&nfho);
	printk("module unload!!!");
}
