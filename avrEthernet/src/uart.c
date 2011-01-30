/*
 * uart.c
 *
 *  Created on: Jul 11, 2010
 *      Author: berni
 */

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>

#define BAUD 38400UL      // Baudrate
// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
#error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch!
#endif

static int uart_putchar(char c, FILE* stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart_getchar(FILE* stream);
static FILE mystdin = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

void uart0_init(void)
{
	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;

	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Asynchron 8N1
	UCSR0B = (1 << TXEN0) | (1 << RXEN0); // tx & rx enable

	stdout = &mystdout;
	stdin = &mystdin;
}

void uart0_putchar(char c)
{
	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = c;
}

void uart0_putstring(char* s)
{
	while (*s)
	{
		uart0_putchar(*s);
		s++;
	}
}

void uart0_putline(char* s)
{
	uart0_putstring(s);
	uart0_putstring("\n\r");
}

char uart0_getchar(void)
{
	while (!(UCSR0A & (1 << RXC0)))
		;
	return (UDR0);
}

void uart0_getline(char* s)
{
	char c = uart0_getchar();
	uint8_t idx = 0;

	while(c != '\r')
	{
		uart0_putchar(c);

		if(c != '\b')
			s[idx++] = c;
		else
			idx--;

		c = uart0_getchar();
	}
	s[idx] = '\0';
	uart0_putstring("\r\n");
}

uint16_t uart0_getuint16(char* prompt)
{
	uart0_putstring(prompt);
	uint16_t val;
	scanf("%x", &val);

	return val;
}


int uart_putchar(char c, FILE* stream)
{
	if(c == '\n')
		uart_putchar('\r', stream);

	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;

	return 0;
}

int uart_getchar(FILE* stream)
{
	char c;
	loop_until_bit_is_set(UCSR0A, RXC0);
	c = UDR0;

	uart_putchar(c, stream);

	return c;
}
