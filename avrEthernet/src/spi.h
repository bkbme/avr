/*
 * spi.h
 *
 *  Created on: Jul 14, 2010
 *      Author: berni
 */

#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

#define CS_LOW 	 PORTB &= ~(1<<PB4);
#define CS_HIGH  PORTB |= (1<<PB4);

void    spi_init_master();
uint8_t spi_write(uint8_t reg);

#endif /* SPI_H_ */
