/*
 * spi.c
 *
 *  Created on: Jul 14, 2010
 *      Author: berni
 */

#include <avr/io.h>
#include "uart.h"
#include <stdio.h>

void spi_init_master()
{
	DDRB = (1<< PB5) | (1<< PB7) | (1<< PB4);
	SPCR = (1<< SPE) | (1<< MSTR);
	SPSR = (1<<SPI2X);
	PORTB |= (1<<PB4);
	printf("spi initialized!\n");
}

uint8_t spi_write(uint8_t reg)
{
	SPDR = reg;
	while(! (SPSR & (1<<SPIF)))
	{

	}

	return SPDR;
}
