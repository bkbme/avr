/*
 * mutex.h
 *
 *  Created on: Oct 3, 2010
 *      Author: berni
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <stdio.h>

typedef struct
{
	int8_t val;
} mutex_data;

void mutex_init(mutex_data* m);
void mutex_lock(mutex_data* m);
void mutex_unlock(mutex_data* m);

#endif /* MUTEX_H_ */
