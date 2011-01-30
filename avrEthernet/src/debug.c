/*
 * debug.c
 *
 *  Created on: Jul 16, 2010
 *      Author: berni
 */


#include <stdio.h>
#include "uart.h"
#include "debug.h"

void debug_dump_mem(uint8_t* mem, uint16_t len)
{
	char text[128];
	*text = '\0';
	uint16_t i=0;
	sprintf(text, "(%4p) ", mem);
	for(;i<len;i++)
	{
		sprintf(text, "%s %2x", text, mem[i] );
		if( (i+1)%8 == 0 )
		{
			uart0_putline(text);
			sprintf(text, "(%4p) ", mem+i);
		}
	}
}

void debug_print_eth_addr(eth_addr* addr)
{
	printf("%x:", addr->bytes[0]);
	printf("%x:", addr->bytes[1]);
	printf("%x:", addr->bytes[2]);
	printf("%x:", addr->bytes[3]);
	printf("%x:", addr->bytes[4]);
	printf("%x", addr->bytes[5]);
}

void debug_print_ip_addr(ip_addr* addr)
{
	printf("%d.", addr->octets[0]);
	printf("%d.", addr->octets[1]);
	printf("%d.", addr->octets[2]);
	printf("%d", addr->octets[3]);
}

