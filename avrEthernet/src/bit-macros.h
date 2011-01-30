/*
 * bit-macros.h
 *
 *  Created on: Sep 11, 2010
 *      Author: berni
 */

#ifndef BIT_MACROS_H_
#define BIT_MACROS_H_

#include <stdlib.h>

#define LO8(x)	 ((uint8_t)(x))
#define HI8(x)	 ((uint8_t)((x)>>8))

#define HTONS(n) (uint16_t)((((uint16_t)(n)) << 8) | (((uint16_t) (n)) >> 8))
#define HTONL(x) ((uint32_t)(((x) & 0xFF000000) >> 24) \
                | (uint32_t)(((x) & 0x00FF0000) >> 8) \
                | (uint32_t)(((x) & 0x0000FF00) << 8) \
                | (uint32_t)(((x) & 0x000000FF) << 24))

#define cbs(bfield, bit) (bfield & (1<<bit))

#endif /* BIT_MACROS_H_ */
