/*
 * buffer.c
 *
 *  Created on: Oct 4, 2010
 *      Author: berni
 */

#include "buffer.h"
#include <stdlib.h>
#include "debug.h"

int8_t buffer_create(buffer_t* buf, uint16_t len)
{
	buf->data = (uint8_t*)malloc(len);
	buf->len = len;
	if(buf->data == NULL) {
		printf("Failed to allocate buffer!\n");
		return -1;
	} else {
		return 0;
	}
}

int8_t buffer_free(buffer_t* buf)
{
	if(buf->data != NULL) {
		free(buf->data);
		buf->len = 0;
		buf->data = NULL;
		return 0;
	} else {
		return -1;
	}
}
