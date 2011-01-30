/*
 * ip.c
 *
 *  Created on: Jul 16, 2010
 *      Author: berni
 */

#include "ethernet.h"
#include <stdlib.h>
#include "ip.h"
#include <stdio.h>

#include "enc28j60.h"
#include "arp.h"
#include <string.h>

/**Generates an ethernet + ip header.
 * @param dest_addr: ip dest address.
 * @param protocol: content for the ip content field
 * @param dest: destination memory in which the header should be written.
 */
void ip_gen_header(ip_addr* dest_addr, uint8_t protocol, uint8_t* dest)
{
	printf("ip_gen_header 0\n");
	eth_addr ether_dest = arp_get_entry(dest_addr)->ether_addr;
	printf("ip_gen_header 1\n");
	ethernet_gen_header(&ether_dest, ETHERTYPE_IP, dest);
	printf("ip_gen_header 2\n");

	ip_header* ip = (ip_header*)(dest+IP_OFFSET);

	ip->version_header_length = (4 << 4) | 5;
	ip->diff_services = 0;
	ip->identification = 0;
	ip->flags_frag_offset = 0x00;
	ip->time_to_live = 64;
	ip->protocol = protocol;
	ip->header_checksum = 0x00;
	ip->source_address.data = LOCAL_IP;
	memcpy(&(ip->destination_address), dest_addr, sizeof(ip_addr));
}

/**Sets the (total) length of the IP Packet.
 * @param total_length: Content of the ip-headers total length field.
 * @param dest: destination memory in which the header should be written.
 */
void ip_set_total_length(uint16_t total_length, uint8_t* dest)
{
	ip_header* ip = (ip_header*)(dest+IP_OFFSET);
	ip->total_length = total_length;
}


