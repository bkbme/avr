/*
 * tcp.c
 *
 *  Created on: Sep 28, 2010
 *      Author: berni
 */

#include "enc28j60.h"
#include "debug.h"
#include "ip.h"
#include "tcp.h"

void tcp_init()
{
	tcp_state = ETS_none;
}

void tcp_listen(uint16_t port)
{
	tcp_state = ETS_listen;
}

