/*
 * main.c
 *
 *  Created on: Jul 11, 2010
 *      Author: berni
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "kernel.h"
#include "cmd.h"
#include "uart.h"
#include "spi.h"
#include "enc28j60.h"

extern kernel_packet_buffer_t	kernel_packet_buffer;

void packet_recv_thread(void* user_data)
{
	uint8_t i,j;
	//buffer_t* tmp = NULL;
	while(1) {
		//tmp = kernel_get_packet();
		if(kernel_packet_buffer.packet_count <= 0) {
			//kernel_go_to_background();
			//printf("rv\n");
		} else {
			printf("Got packet!\n");
		}

#if 0
		if(i>254) {
			if(j>254) {
				printf("prt\n");
				i=0;
				j=0;
			}
			j++;
		}
		i++;
#endif

	}
}

void kernel_packet_thread(void* user_data)
{
	uint8_t i,j;
	while(1) {
		if(kernel_packet_buffer.tmp_storage != NULL) {
			//kernel_go_to_background();
			printf("kthread\n");

			//mutex_lock(&(kernel_packet_buffer.mutex));

			kernel_packet_buffer.packets[kernel_packet_buffer.packet_count] =
					kernel_packet_buffer.tmp_storage;
			kernel_packet_buffer.tmp_storage = NULL;
			kernel_packet_buffer.packet_count++;
			printf("p\n");

			//mutex_unlock(&(kernel_packet_buffer.mutex));
		}

#if 0
		if(i>254) {
			if(j>254) {
				printf("kpt\n");
				i=0;
				j=0;
			}
			j++;
		}
		i++;
#endif

	}
}

int main()
{
	sei();
	uart0_init();
	spi_init_master();
	kernel_init();
	enc_init();

	printf("System started!\n");
	//arp_init();
	//icmp_init();

	kernel_register_thread(cmd_thread, 1, 300);
	kernel_register_thread(packet_recv_thread, 1, KERNEL_MIN_STACK_SIZE+20);
	kernel_register_thread(kernel_packet_thread, 1, KERNEL_MIN_STACK_SIZE+20);

	kernel_exec();

	return 0;
}


