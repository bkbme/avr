/*
 * icmp.c
 *
 *  Created on: Sep 28, 2010
 *      Author: berni
 */

#include "icmp.h"
#include "enc28j60.h"
#include "arp.h"
#include "ip.h"
#include "buffer.h"
#include <stdio.h>

void stack_icmp_callback(uint8_t* data, uint16_t len)
{
	if(len >= ICMP_PLEN) {
		ethernet_header* eh = (ethernet_header*)data;
		if(eh->len_field == ETHERTYPE_IP) {
			ip_header* ih = (ip_header*)(data+IP_OFFSET);
			if(ih->protocol == IP_PROTOCOL_ICMP) {
				icmp_header* icmph = (icmp_header*)(data+IP_DATA_OFFSET);
				if(icmph->type_code == ICMP_TYPE_ECHO_REQUEST) {
					printf("ICMP ECHO REQUEST\n");
				} else if(icmph->type_code == ICMP_TYPE_ECHO_REPLY) {
					printf("ICMP ECHO REPLY\n");
				}
			}
		}
	}
}

void icmp_init()
{
	//stack_register_function(stack_icmp_callback);
}

void icmp_gen_header(uint16_t icmp_type_code, uint8_t* dest)
{
	icmp_header* icmp = (icmp_header*)(dest+IP_DATA_OFFSET);
	icmp->type_code = icmp_type_code;
	icmp->id = 0xfefe;
	icmp->sequence = icmp_sequence_num;
}

void icmp_send_packet(ip_addr* dest_addr, uint16_t icmp_type_code, uint8_t* dest)
{

	buffer_t* buf = NULL;
	buffer_create(buf, ICMP_PLEN);
	ip_gen_header(dest_addr, IP_PROTOCOL_ICMP, buf->data);
	ip_set_total_length(28, buf->data);
	icmp_gen_header(ICMP_TYPE_ECHO_REQUEST, buf->data);
}

