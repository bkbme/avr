/*
 * ethernet.c
 *
 *  Created on: Jul 15, 2010
 *      Author: berni
 */
#include "ethernet.h"
#include <stdio.h>

/**Generates a ethernet header.
 * @param addr: Destination ethernet address
 * @param length: Content of the ethernet length field.
 * @param dest: Pointer to the location where the packet should be built.
 */
void ethernet_gen_header(eth_addr* addr, uint16_t length, uint8_t* dest)
{
	printf("ethernet_gen_header 0\n");
	ethernet_header* eh = (ethernet_header*)dest;
	SET_MAC((&(eh->src_addr)));
	eh->dest_addr = *addr;
	eh->len_field = length;
	printf("ethernet_gen_header 1\n");
}

