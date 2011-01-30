/*
 * debug.h
 *
 *  Created on: Jul 16, 2010
 *      Author: berni
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "ip.h"
#include "ethernet.h"

void debug_dump_mem(uint8_t* mem, uint16_t len);

void debug_print_eth_addr(eth_addr* addr);
void debug_print_ip_addr(ip_addr* addr);

#endif /* DEBUG_H_ */
