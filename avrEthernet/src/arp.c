/*
 * arp.c
 *
 *  Created on: Sep 18, 2010
 *      Author: berni
 */

#include "arp.h"
#include "ip.h"
#include "string.h"
#include "debug.h"
#include <stdio.h>
#include "ethernet.h"
#include "enc28j60.h"
#include <util/delay.h>
#include "buffer.h"

void arp_add_to_table(arp_header* ah);
int8_t arp_table_contains(ip_addr* ip);

/**Creates ARP Request.
 * @param addr: eth addr from which the ip should be requested.
 * @param dest:	Pointer where the packet data should be written.
 */
void arp_request(ip_addr* addr, uint8_t* dest)
{
	arp_header* ah = (arp_header*)dest;
	ah->hType = ARP_HTYPE_ETHERNET;
	ah->pType = ETHERTYPE_IP;
	ah->hlen  = 6;
	ah->plen  = 4;
	ah->oper  = ARP_OPCODE_REQUEST;
	SET_MAC((&(ah->sha)));
	ah->spa   = LOCAL_IP;
	ah->tha.words[0] = 0;
	ah->tha.words[1] = 0;
	ah->tha.words[2] = 0;
	ah->tpa			 = addr->data;
}

#if 0
/**Callback for every incoming packet.
 *
 */
void arp_callback(uint8_t* data, uint16_t len)
{
	printf("arp_callback\n");
	ethernet_header* eh = (ethernet_header*)data;
	if(eh->len_field == ETHERTYPE_ARP)
	{
		arp_header* ah = (arp_header*)(data+ETH_HLEN);
		if(ah->oper == ARP_OPCODE_REPLY)
		{
			arp_add_to_table(ah);
		}
		else if(ah->oper == ARP_OPCODE_REQUEST)
		{
			arp_add_to_table(ah);
			//Send arp reply..
			eth_addr source_ether = ah->sha;
			ip_addr  source_ip;
			source_ip.data = ah->spa;
			ethernet_gen_header(&(source_ether), ETHERTYPE_ARP, eth_data);
			arp_header* ah_rep = (arp_header*)(eth_data+ETH_HLEN);
			ah_rep->hType = ARP_HTYPE_ETHERNET;
			ah_rep->pType = ETHERTYPE_IP;
			ah_rep->hlen = 6;
			ah_rep->plen = 4;
			ah_rep->oper = ARP_OPCODE_REPLY;
			SET_MAC((&(ah_rep->sha)));
			ah_rep->spa = LOCAL_IP;
			ah_rep->tpa = source_ip.data;
			ah_rep->tha = source_ether;
			enc_prepare_ethernet_packet((ethernet_header*)eth_data);
			enc_add_packet_data(eth_data+ETH_HLEN, sizeof(arp_header));
			enc_send_packet();
		}
	}
}
#endif

void arp_send_request(ip_addr* ip)
{
	eth_addr brdcast;
	brdcast.words[0] = HTONS(0xffff);
	brdcast.words[1] = HTONS(0xffff);
	brdcast.words[2] = HTONS(0xffff);

	buffer_t* buf = NULL;
	buffer_create(buf, ARP_PLEN);
	ethernet_gen_header(&brdcast, ETHERTYPE_ARP, buf->data);
	ethernet_header* eh = (ethernet_header*)buf->data;
	arp_request(ip, buf->data+sizeof(ethernet_header));
	enc_prepare_ethernet_packet(eh);
	enc_add_packet_data(buf->data+sizeof(ethernet_header), sizeof(arp_header));
	enc_send_packet();
}

/**Checks arp packet and adds info to table.
 * If an entry for the hw-addr exists, the entry is updated.
 */
void arp_add_to_table(arp_header* ah)
{
	int8_t retVal;
	retVal = arp_table_contains((ip_addr*)&(ah->spa));
	if(retVal == -1)
	{
		retVal = arp_table_pos;
		arp_table_pos++;
	}
	memcpy(&(arp_table[retVal].ether_addr), &(ah->sha), sizeof(eth_addr));
	memcpy(&(arp_table[retVal].ip_addr), &(ah->spa), sizeof(ip_addr));
}

void arp_init()
{
	arp_table_pos = 0;
	//stack_register_function(arp_callback);
}

void arp_dump_table()
{
	printf("------ ARP TABLE BEGIN -------\n");
	printf("(HW)\t\t\t(IP)\n");
	uint8_t p=0;
	for(;p<arp_table_pos; p++)
	{
		debug_print_eth_addr(&(arp_table[p].ether_addr));
		printf("\t");
		debug_print_ip_addr(&(arp_table[p].ip_addr));
		printf("\n");
	}
	printf("------ ARP TABLE END --------\n");
}

/**Checks if a arp entry exists for the supplied ip_addr.
 * @return If no entry exists for ip_addr, -1 is returned.
 * 		   If such an entry exists, the position of the entry is returned.
 */
int8_t arp_table_contains(ip_addr* ip)
{
	uint8_t p=0;
	for(;p<arp_table_pos; p++)
	{
		if(arp_table[p].ip_addr.data == ip->data)
		{
			return p;
		}
	}

	return -1;
}

/**Gets an entry out of the arp table.
 *
 */
arp_table_entry* arp_get_entry(ip_addr* ip)
{
	return NULL;
	printf("arp_get_entry 0\n");
	int8_t pos = arp_table_contains(ip);
	if(pos == -1)
	{
		arp_send_request(ip);
		while(pos == -1)
		{
			printf("arp_get_entry 1\n");
			pos = arp_table_contains(ip);
		}
	}

	if(pos >= 0 && pos < arp_table_pos)
	{
		return arp_table+pos;
	}
	else
	{
		return NULL;
	}
}


