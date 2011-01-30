/*
 * buffer.h
 *
 *  Created on: Oct 4, 2010
 *      Author: berni
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdio.h>

typedef struct
{
	uint8_t* data;
	uint16_t len;
} buffer_t;

int8_t buffer_create(buffer_t* buf, uint16_t len);
int8_t buffer_free(buffer_t* buf);


#endif /* BUFFER_H_ */
