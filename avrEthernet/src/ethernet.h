/*
 * ethernet.h
 *
 *  Created on: Jul 15, 2010
 *      Author: berni
 */

#ifndef ETHERNET_H_
#define ETHERNET_H_

#include <avr/io.h>
#include "bit-macros.h"

typedef union
{
	uint8_t  bytes[6];
	uint16_t words[3];
} eth_addr;

typedef struct
{
	eth_addr	dest_addr;
	eth_addr 	src_addr;
	uint16_t 	len_field;
} ethernet_header;

#define ETH_HLEN 14

#define NET_MAX_ETH_FRALEN	1500

#define ENC_MAC_0	0xfe
#define ENC_MAC_1	0xfe
#define ENC_MAC_2	0xfe
#define ENC_MAC_3	0xfe
#define ENC_MAC_4	0xfe
#define ENC_MAC_5	0x01

#define SET_MAC(ea)		ea->bytes[0]=ENC_MAC_0;ea->bytes[1]=ENC_MAC_1;ea->bytes[2]=ENC_MAC_2;ea->bytes[3]=ENC_MAC_3;ea->bytes[4]=ENC_MAC_4;ea->bytes[5]=ENC_MAC_5;

/* Protocol Types */
#define ETHERTYPE_IP 			HTONS(0x0800) // 	Internet Protocol, Version 4 (IPv4)
#define ETHERTYPE_ARP			HTONS(0x0806) //	Address Resolution Protocol (ARP)
#define ETHERTYPE_WOL_MAGIC		HTONS(0x0842) //	Wake-on-LAN Magic Packet, as used by ether-wake and Sleep Proxy Service
#define ETHERTYPE_RARP			HTONS(0x8035) // 	Reverse Address Resolution Protocol (RARP)
#define ETHERTYPE_VLAN_TAGGED	HTONS(0x8100) //	VLAN-tagged frame (IEEE 802.1Q)
#define ETHERTYPE_IPv6			HTONS(0x86DD) // 	Internet Protocol, Version 6 (IPv6)
#define ETHERTYPE_PPPoE_DISC	HTONS(0x8863) // 	PPPoE Discovery Stage
#define ETHERTYPE_PPPoE_SESS	HTONS(0x8864) // 	PPPoE Session Stage

void ethernet_gen_header(eth_addr* addr, uint16_t length, uint8_t* dest);

#endif /* ETHERNET_H_ */
