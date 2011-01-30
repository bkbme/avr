/*
 * cmd.c
 *
 *  Created on: Oct 9, 2010
 *      Author: berni
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "enc28j60.h"
#include "uart.h"
#include "ethernet.h"
#include "debug.h"
#include "ip.h"
#include "spi.h"
#include "arp.h"
#include "icmp.h"
#include "udp.h"

static void cmd_dumpbuf(uint16_t start, uint16_t end)
{
	char text[128];
	sprintf(text, "(%4x) ", start);
	for(;start != end; start++)
	{
		sprintf(text, "%s %2x", text, enc_rbm(start));
		if((start+1)%8 == 0)
		{
			uart0_putline(text);
			sprintf(text, "(%4x) ", start+1);
		}
	}
	if(*text)
		uart0_putline(text);
}

void cmd_thread(void* user_data)
{
	char text[32];
	int val;
	uint8_t settings=0;

	while(1)
	{
		uart0_getline(text);
		if(settings == 0) {
			if(!strcmp(text, "t"))
			{
				printf("Hello World!");
			}
			else if(!strcmp(text, "dumpbank"))
			{
				scanf("%u", &val);
				enc_switch_bank(val);
				printf("bank %d\n", val);
				uint8_t i=0;
				uint8_t addr;
				for(;i<32; i++)
				{
					if(i%4 == 0)	printf("\n");
					if(val == 2)
					{
						addr = rdef(val, i, 1);
					}
					else
					{
						addr = rdef(val, i, 0);
					}
					printf("(%2x): %2x\t", i, enc_rcr(addr));
				}
				printf("\n");
			}
			else if(!strcmp(text, "dumpbuf"))
			{
				uint16_t start = uart0_getuint16("start: ");
				uint16_t end = uart0_getuint16("end: ");
				*text = '\0';
				cmd_dumpbuf(start, end);
			}
			else if(!strcmp(text, "recvpak"))
			{
				enc_recv_packet();
			}
			else if(!strcmp(text, "arp_get"))
			{
				ip_addr adr;
				adr.data = IPCONV(192,168,88,1);
				arp_table_entry* arp_entry = arp_get_entry(&adr);

				if(arp_entry == NULL)
				{
					printf("arp is NULL\n");
				}
				else
				{
					printf("arp is ");
					debug_print_eth_addr(&(arp_entry->ether_addr));
					printf("\n");
				}
			}
			else if(!strcmp(text, "arp_dump"))
			{
				arp_dump_table();
			}
			else if(!strcmp(text, "udp"))
			{
				settings = 1;

			}
		} else if(settings == 1) {
			ip_addr ip_dest;
			ip_dest.data = IPCONV(192,168,88,1);
			udp_send_packet(text, strlen(text), &ip_dest, 6667, 6667 );
		}

	}
}
