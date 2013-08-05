#include <linux/init.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/inet.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <net/checksum.h>
#include <linux/inet.h>
#include <linux/moduleparam.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#undef __KERNEL__
#include <linux/netfilter_ipv4.h>
#define __KERNEL__
#include "p.h"

static int cmd;
static int val;
static int num;
static char dip[MAX_IP];
static int dprt;
static char sip[MAX_IP];
static int sprt;

int count;
int state;

static struct ctl_table sysctl_net[] = {
        {
                .procname       = "cmd",
                .data           = &cmd,
                .maxlen         = sizeof(int),
                .mode           = 0777,
                .proc_handler   = &proc_dointvec,
        },
        {
                .procname       = "val",
                .data           = &val,
                .maxlen         = sizeof(int),
                .mode           = 0777,
                .proc_handler   = &proc_dointvec,
        },
        {
                .procname       = "num",
                .data           = &num,
                .maxlen         = sizeof(int),
                .mode           = 0777,
                .proc_handler   = &proc_dointvec,
        },
        {
                .procname       = "dprt",
                .data           = &dprt,
                .maxlen         = sizeof(int),
                .mode           = 0777,
                .proc_handler   = &proc_dointvec,
        },
        {
                .procname       = "sprt",
                .data           = &sprt,
                .maxlen         = sizeof(int),
                .mode           = 0777,
                .proc_handler   = &proc_dointvec,
        },
        {
                .procname       = "dip",
                .data           = &dip,
                .maxlen         = MAX_IP,
                .mode           = 0777,
                .proc_handler   = &proc_dostring,
        },
        {
                .procname       = "sip",
                .data           = &sip,
                .maxlen         = MAX_IP,
                .mode           = 0777,
                .proc_handler   = &proc_dostring,
        }
};

static struct ctl_table ctltable = {
        .procname       = "project",
        .mode           = 0555,
        .child          = sysctl_net,
};

static struct ctl_table_header * ctl_header;
static struct nf_hook_ops nfho;

unsigned int hook_func(unsigned int hooknum, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *))
{
	struct iphdr *iph = NULL;
	struct tcphdr *tcph = NULL;
	
	iph = ip_hdr(skb); // access ip header.
	
	if ((iph->protocol == IPPROTO_TCP) )
	{
		//printk("TCP get \n");
		tcph = (struct tcphdr *)(skb_network_header(skb) + ip_hdrlen(skb)); // access tcp header.
		
		printk("CMD: cmd %d, val %d, num %d.\n",cmd,val,num);
		printk("INFO: Source IP Address: %u.%u.%u.%u.\n",NIPQUAD(iph->saddr));
		printk("INFO: Destination IP Address: %u.%u.%u.%u.\n",NIPQUAD(iph->daddr));
		printk("INFO: Source Port: %u.\n",tcph->source);
		printk("INFO: Destination Port: %u.\n",tcph->dest);
		printk("INFO: window: %u.\n",tcph->window);
		//printk("len = %d\n", skb->len);
		switch( cmd )
		{
			case 0:
				break;
			case 1:
			{
				count = num;
				state = 1;
				cmd = 0;
			}break;
			case 2:
			{
				count = val;
				state = 2;
				cmd = 0;
			}break;
		}
		
		switch( state )
		{
			case 0:
				break;
			case 1:
			{
				if( num == -1 )
					state = 0;
				
				if( iph->saddr == in_aton( sip ) && iph->daddr == in_aton( dip ) )
				{
					if( tcph->source == sprt && tcph->dest == dprt )
					{
						if( num != 0 )
							count--;
						tcph->window = val;
						
						if( num != 0 && count == 0 )
							state = 0;
					}
				}
				
				
				int datalen = skb->len - iph->ihl*4;
				tcph->check = 0;
				tcph->check = tcp_v4_check(datalen, iph->saddr, iph->daddr, csum_partial((char *)tcph, datalen, 0));
				
				printk("CHANGE: window: %u.\n",tcph->window);
			}break;
			case 2:
			{
				if( val == -1 )
					state = 0;
				
				if( iph->saddr == in_aton( sip ) && iph->daddr == in_aton( dip ) )
				{
					if( tcph->source == sprt && tcph->dest == dprt )
					{
						if( val != 0 )
							count--;
						
						if( val != 0 && count == 0 )
							state = 0;
							
					}
				}
				
				printk("CHANGE: Discard this.\n");
				return NF_DROP;
			}break;
		}
		
	}
	
	return NF_ACCEPT;
}
static int __init hook_init(void)
{
		nfho.hook     = hook_func;         /* Handler function */
		nfho.hooknum  = NF_IP_PRE_ROUTING; 
		nfho.pf       = PF_INET;
		nfho.priority = NF_IP_PRI_FIRST;   /* Make our function first */
		
		count = 0;
		cmd = 0;
		num = 0;
		state = 0;
		
        nf_register_hook(&nfho);
		ctl_header = register_sysctl_table(&ctltable);

        return 0;
}

static void __exit hook_exit(void)
{
        unregister_sysctl_table(ctl_header);
		nf_unregister_hook(&nfho);
}

module_init(hook_init);
module_exit(hook_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mike");