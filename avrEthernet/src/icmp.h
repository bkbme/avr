/*
 * icmp.h
 *
 *  Created on: Sep 28, 2010
 *      Author: berni
 */

#ifndef ICMP_H_
#define ICMP_H_

#include "ip.h"
#include "ethernet.h"
#include <avr/io.h>

typedef struct
{
	uint16_t 	type_code;
	uint16_t	checksum,
				id,
				sequence;
} icmp_header;

static uint16_t icmp_sequence_num;

#define ICMP_HLEN					8
#define ICMP_PLEN					(IP_HLEN+ETH_HLEN+ICMP_HLEN)

#define ICMP_TYPE_ECHO_REPLY		HTONS(0)	//Echo reply
#define ICMP_TYPE_ECHO_REQUEST		HTONS(8<<8)	//Echo request

void icmp_init();
void icmp_send_packet(ip_addr* dest_addr, uint16_t icmp_type_code, uint8_t* dest);

#endif /* ICMP_H_ */
