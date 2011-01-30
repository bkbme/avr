/*
 * mutex.c
 *
 *  Created on: Oct 3, 2010
 *      Author: berni
 */

#include "mutex.h"
#include <util/atomic.h>
#include "kernel.h"

void mutex_init(mutex_data* m)
{
	m->val = 0;
}

void mutex_lock(mutex_data* m)
{
	while(m->val != 0) {
		//kernel_go_to_background();
	}

	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		if(m->val == 0) {
			m->val = 1;
		}
	}
}

void mutex_unlock(mutex_data* m)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		m->val = 0;
	}
}
