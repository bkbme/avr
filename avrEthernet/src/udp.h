/*
 * udp.h
 *
 *  Created on: Sep 28, 2010
 *      Author: berni
 */

#ifndef UDP_H_
#define UDP_H_

#include "ip.h"
#include "ethernet.h"

typedef struct
{
	uint16_t	source_port;
	uint16_t	destination_port;
	uint16_t	length;
	uint16_t	checksum;
} udp_header;

#define UDP_HLEN 				8
#define UDP_PLEN				(ETH_HLEN+IP_HLEN+UDP_HLEN)
#define UDP_OFFSET				IP_DATA_OFFSET
#define UDP_DATA_OFFSET			UDP_OFFSET+UDP_HLEN
#define UDP_OFFSET_CHECKSUM_LO8	UDP_OFFSET+7
#define UDP_OFFSET_CHECKSUM_HI8	UDP_OFFSET+8

void udp_send_packet(uint8_t* data, uint16_t len, ip_addr* ip_dest,
					 uint16_t src_port, uint16_t dest_port);

#endif /* UDP_H_ */
