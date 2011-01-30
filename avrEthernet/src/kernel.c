/*
 * kernel.c
 *
 *  Created on: Oct 6, 2010
 *      Author: berni
 */
#include "kernel.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include "enc28j60.h"

kernel_thread_control_block*	kernel_stack_pointer;
kernel_thread_control_block*	kernel_threads[5];
uint8_t 						kernel_threads_pos;
uint8_t							kernel_running_thread;

kernel_packet_buffer_t	kernel_packet_buffer;

#define kernel_save_context()							\
asm volatile (	"push	r0						\n\t"	\
				"in		r0, __SREG__			\n\t"	\
				"cli							\n\t"	\
				"push	r0						\n\t"	\
				"push	r1						\n\t"	\
				"clr	r1						\n\t"	\
				"push	r2						\n\t"	\
				"push	r3						\n\t"	\
				"push	r4						\n\t"	\
				"push	r5						\n\t"	\
				"push	r6						\n\t"	\
				"push	r7						\n\t"	\
				"push	r8						\n\t"	\
				"push	r9						\n\t"	\
				"push	r10						\n\t"	\
				"push	r11						\n\t"	\
				"push	r12						\n\t"	\
				"push	r13						\n\t"	\
				"push	r14						\n\t"	\
				"push	r15						\n\t"	\
				"push	r16						\n\t"	\
				"push	r17						\n\t"	\
				"push	r18						\n\t"	\
				"push	r19						\n\t"	\
				"push	r20						\n\t"	\
				"push	r21						\n\t"	\
				"push	r22						\n\t"	\
				"push	r23						\n\t"	\
				"push	r24						\n\t"	\
				"push	r25						\n\t"	\
				"push	r26						\n\t"	\
				"push	r27						\n\t"	\
				"push	r28						\n\t"	\
				"push	r29						\n\t"	\
				"push	r30						\n\t"	\
				"push	r31						\n\t"	\
				"lds	r26, kernel_stack_pointer		\n\t"	\
				"lds	r27, kernel_stack_pointer + 1	\n\t"	\
				"in		r0, 0x3d				\n\t"	\
				"st		x+, r0					\n\t"	\
				"in		r0, 0x3e				\n\t"	\
				"st		x+, r0					\n\t"	\
			);

#define kernel_restore_context()						\
asm volatile (	"lds	r26, kernel_stack_pointer		\n\t"	\
				"lds	r27, kernel_stack_pointer + 1	\n\t"	\
				"ld		r28, x+					\n\t"	\
				"out	__SP_L__, r28			\n\t"	\
				"ld		r29, x+					\n\t"	\
				"out	__SP_H__, r29			\n\t"	\
				"pop	r31						\n\t"	\
				"pop	r30						\n\t"	\
				"pop	r29						\n\t"	\
				"pop	r28						\n\t"	\
				"pop	r27						\n\t"	\
				"pop	r26						\n\t"	\
				"pop	r25						\n\t"	\
				"pop	r24						\n\t"	\
				"pop	r23						\n\t"	\
				"pop	r22						\n\t"	\
				"pop	r21						\n\t"	\
				"pop	r20						\n\t"	\
				"pop	r19						\n\t"	\
				"pop	r18						\n\t"	\
				"pop	r17						\n\t"	\
				"pop	r16						\n\t"	\
				"pop	r15						\n\t"	\
				"pop	r14						\n\t"	\
				"pop	r13						\n\t"	\
				"pop	r12						\n\t"	\
				"pop	r11						\n\t"	\
				"pop	r10						\n\t"	\
				"pop	r9						\n\t"	\
				"pop	r8						\n\t"	\
				"pop	r7						\n\t"	\
				"pop	r6						\n\t"	\
				"pop	r5						\n\t"	\
				"pop	r4						\n\t"	\
				"pop	r3						\n\t"	\
				"pop	r2						\n\t"	\
				"pop	r1						\n\t"	\
				"pop	r0						\n\t"	\
				"out	__SREG__, r0			\n\t"	\
				"pop	r0						\n\t"	\
			);


/**Inits the kernel.
 * Must be called before registering threads or executing the kernel loop.
 */
void kernel_init()
{
	kernel_threads_pos = 0;
	kernel_running_thread = 0;
	TIMSK0 |= (1<<TOIE0);
	kernel_packet_buffer.tmp_storage = NULL;
	kernel_packet_buffer.packet_count = 0;
	mutex_init(&(kernel_packet_buffer.mutex));
}

/**Starts the kernel timer.
 * Only used internally.
 */
void kernel_start_timer()
{
	TCCR0B |= (1<<CS00) | (1<<CS02);
	sei();
}

/**Registers a thread to be scheduled by the kernel.
 * @param function: function pointer to the thread function.
 * @param priority: Thread priority 0 is highest priority, 255 lowest.
 * @param stack_size: Number of variables used in thread + KERNEL_MIN_STACK_SIZE.
 */
int8_t kernel_register_thread(	kernel_thread_callback_t function,
								uint8_t priority,
								uint16_t stack_size)
{
	kernel_threads[kernel_threads_pos] =
			(kernel_thread_control_block*)malloc(sizeof(kernel_thread_control_block));
	kernel_thread_control_block* cb = kernel_threads[kernel_threads_pos];

	if(kernel_threads[kernel_threads_pos] == NULL) return -1;

	cb->priority = priority;
	cb->priority_ticks = priority;
	cb->stack = (unsigned char*)malloc(stack_size);

	if(cb->stack == NULL) {
		free(kernel_threads[kernel_threads_pos]);
		return -2;
	}

	cb->function = function;
	cb->top_of_stack = cb->stack+(stack_size-1);
	unsigned char* tos = (cb->top_of_stack);
	unsigned short function_address = (unsigned short)function;

	*tos = (function_address & (unsigned short)0x00ff);
	tos--;
	function_address >>= 8;
	*tos = (function_address & (unsigned short)0x00ff);
	tos--;

	*tos = 0x00; /*R0*/
	tos--;
	*tos = 0x80; /*Here lies the flags register content. It says that IRQ is enabled. */
	tos--;

	*tos = 0x00; /*R1*/
	tos--;
	uint8_t i=2;
	for(;i<32;i++) { /* R2 - R22 */
		*tos = i;
		tos--;
	}

	cb->top_of_stack = tos;

	kernel_threads_pos++;

	return 0;
}

/**Executes the kernel loop.
 *
 */
void kernel_exec()
{
	kernel_start_timer();
	kernel_stack_pointer = kernel_threads[kernel_running_thread];
	kernel_restore_context();
	asm volatile ( "reti" );
}

/**Switches kernel_stack_pointer to the next thread.
 * Only used internally.
 */
static inline void kernel_switch_context()
{
	if(kernel_running_thread < kernel_threads_pos-1) {
		kernel_running_thread++;
	} else {
		kernel_running_thread = 0;
	}

	kernel_stack_pointer = kernel_threads[kernel_running_thread];
}

/**Runs the kernel scheduler.
 * Saves context.
 * Checks thread priority.
 * Swiches context.
 * Only used internally.
 */
static inline void kernel_exec_scheduler()
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		kernel_save_context();

		if(kernel_stack_pointer->priority_ticks == 0) {
			kernel_stack_pointer->priority_ticks = kernel_stack_pointer->priority;
			kernel_switch_context();
		} else {
			kernel_stack_pointer->priority_ticks--;
		}

		kernel_restore_context();
	}
}

/**Sends a thread, which is scheduled by the kernel into background.
 * Could be used in waiting loops to handoff to the next thread.
 */
void kernel_go_to_background() __attribute__( ( naked ) );
void kernel_go_to_background()
{
	kernel_exec_scheduler();

	asm volatile ( "reti" );
}

void SIG_OVERFLOW0(void) __attribute__( ( signal, naked ) );
void SIG_OVERFLOW0(void)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		kernel_save_context();

		if(kernel_stack_pointer->priority_ticks == 0) {
			kernel_stack_pointer->priority_ticks = kernel_stack_pointer->priority;
			if(kernel_running_thread < kernel_threads_pos-1) {
				kernel_running_thread++;
			} else {
				kernel_running_thread = 0;
			}

			kernel_stack_pointer = kernel_threads[kernel_running_thread];
		} else {
			kernel_stack_pointer->priority_ticks--;
		}

		kernel_restore_context();
	}

	asm volatile ( "reti" );
}

void kernel_add_packet(buffer_t* pack)
{
	if(kernel_packet_buffer.tmp_storage != NULL) {
		buffer_free(pack);
	} else {
		kernel_packet_buffer.tmp_storage = pack;
		printf("padd 0x%p\n", kernel_packet_buffer.tmp_storage);
	}
}

buffer_t* kernel_get_packet()
{
	//mutex_lock(&(kernel_packet_buffer.mutex));

	if(kernel_packet_buffer.packet_count <= 0) return NULL;

	kernel_packet_buffer.packet_count--;
	buffer_t* retVal = kernel_packet_buffer.packets[kernel_packet_buffer.packet_count];

	//mutex_unlock(&(kernel_packet_buffer.mutex));

	return retVal;
}

