/*
 * uart.h
 *
 *  Created on: Jul 11, 2010
 *      Author: berni
 */

#ifndef UART_H_
#define UART_H_

void uart0_init(void);
void uart0_putchar( char c );
char uart0_getchar( void );
void uart0_getline(char* s);
void uart0_putstring(char* s);
void uart0_putline(char* s);

uint16_t uart0_getuint16(char* prompt);


#endif /* UART_H_ */
