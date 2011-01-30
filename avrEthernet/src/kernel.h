/*
 * kernel.h
 *
 *  Created on: Oct 6, 2010
 *      Author: berni
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdio.h>
#include "mutex.h"
#include "buffer.h"

#define KERNEL_MIN_STACK_SIZE	85

typedef void(*kernel_thread_callback_t)(void* user_data);

typedef struct
{
	unsigned char*				top_of_stack;
	unsigned char*				stack;
	kernel_thread_callback_t	function;
	uint8_t						priority,
								priority_ticks;
} kernel_thread_control_block;

typedef struct
{
	mutex_data	mutex;
	buffer_t*	packets[10];
	buffer_t*	tmp_storage;
	uint8_t		packet_count;
} kernel_packet_buffer_t;

void 		kernel_init();
void 		kernel_exec();
int8_t 		kernel_register_thread(	kernel_thread_callback_t function,
									uint8_t priority,
									uint16_t stack_size);
//void 		kernel_go_to_background();
void 		kernel_add_packet(buffer_t* pack);
buffer_t* 	kernel_get_packet();

#endif /* KERNEL_H_ */
