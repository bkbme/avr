/*
 * ip.h
 *
 *  Created on: Jul 16, 2010
 *      Author: berni
 */

#ifndef IP_H_
#define IP_H_

#include <avr/io.h>

typedef union
{
	uint32_t data;
	uint8_t octets[4];
} ip_addr;

typedef struct
{
	uint8_t 	version_header_length;
	uint8_t 	diff_services;
	uint16_t 	total_length;
	uint16_t 	identification;
	uint16_t 	flags_frag_offset;
	uint8_t 	time_to_live;
	uint8_t		protocol;
	uint16_t    header_checksum;
	ip_addr		source_address;
	ip_addr		destination_address;
} ip_header;

#define IP_HLEN 				20
#define IP_OFFSET 				ETH_HLEN
#define IP_DATA_OFFSET			IP_OFFSET+IP_HLEN
#define IP_OFFSET_CHECKSUM_LO8 	IP_OFFSET+12
#define IP_OFFSET_CHECKSUM_HI8 	IP_OFFSET+11

#define IPCONV(a,b,c,d) (((((uint32_t)a))|(((uint32_t)b)<<8)|(((uint32_t)c)<<16)|((uint32_t)d<<24)))

#define LOCAL_IP IPCONV(192,168,88,254)

#define IP_PROTOCOL_UDP		0x11
#define IP_PROTOCOL_TCP		0x06
#define IP_PROTOCOL_ICMP	0x01


void ip_gen_header(ip_addr* dest_addr, uint8_t protocol, uint8_t* dest);
void ip_set_total_length(uint16_t total_length, uint8_t* dest);


#endif /* IP_H_ */
