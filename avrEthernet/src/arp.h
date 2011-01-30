/*
 * arp.h
 *
 *  Created on: Sep 18, 2010
 *      Author: berni
 */

#ifndef ARP_H_
#define ARP_H_

#include "ethernet.h"
#include "ip.h"

typedef struct
{
	uint16_t	hType; //Hardware Type
	uint16_t	pType; //Protocol Type
	uint8_t		hlen,  //Hardware Len
				plen;  //Protocol Len
	uint16_t	oper; //ARP opcode
	eth_addr	sha; //Sender Hardware address
	uint32_t	spa; //Sender Protocol address
	eth_addr	tha; //Target Hardware address
	uint32_t	tpa; //Target Protocol address

} arp_header;

typedef struct
{
	eth_addr ether_addr;
	ip_addr  ip_addr;
} arp_table_entry;

/* ARP Opcodes */
#define ARP_OPCODE_RES			HTONS(0) //	Reserved	[RFC5494]
#define ARP_OPCODE_REQUEST		HTONS(1) //	REQUEST	[RFC826][RFC5227]
#define ARP_OPCODE_REPLY		HTONS(2) //	REPLY	[RFC826][RFC5227]
#define ARP_OPCODE_REQRES		HTONS(3) //	request Reverse	[RFC903]
#define ARP_OPCODE_REPRES		HTONS(4) //	reply Reverse	[RFC903]
#define ARP_OPCODE_DRARP_REQ	HTONS(5) //	DRARP-Request	[RFC1931]
#define ARP_OPCODE_DRARP_REP	HTONS(6) //	DRARP-Reply	[RFC1931]
#define ARP_OPCODE_DRARP_ERR	HTONS(7) //	DRARP-Error	[RFC1931]
#define ARP_OPCODE_INARP_REQ	HTONS(8) //	InARP-Request	[RFC1293]
#define ARP_OPCODE_INARP_REP	HTONS(9) //	InARP-Reply	[RFC1293]
#define ARP_OPCODE_NAK			HTONS(10) //	ARP-NAK	[RFC1577]

/* Hardware Type */
#define ARP_HTYPE_ETHERNET		HTONS(1)

#define ARP_MAX_TABLE_LEN		5

#define ARP_HLEN				26
#define ARP_PLEN				(ETH_HLEN+IP_HLEN+ARP_HLEN)


void 				arp_request(ip_addr* addr, uint8_t* dest);
void 				arp_dump_table();
void 				arp_init();
arp_table_entry* 	arp_get_entry(ip_addr* ip);

static arp_table_entry arp_table[ARP_MAX_TABLE_LEN];
static uint8_t 		   arp_table_pos;

#endif /* ARP_H_ */
