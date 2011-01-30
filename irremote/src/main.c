/*
 * main.c
 *
 *  Created on: May 2, 2010
 *      Author: berni
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#include "samsung_protocol.h"
#include "usbdrv.h"

void initPWM();


usbMsgLen_t usbFunctionSetup(uchar data[8])
{
  usbRequest_t *rq = (void *)data;
  static uchar replyBuf[2];
  usbMsgPtr = replyBuf;
  uchar len = 0;

  switch(rq->bRequest)
  {
    //ECHO
    case 0:
      replyBuf[0] = rq->wValue.bytes[0];
      replyBuf[1] = rq->wValue.bytes[1];
      len = 2;
      break;

    //Restart System by causing watchdog to restart
    case 1:
      wdt_disable();
      wdt_enable(WDTO_15MS);
      _delay_ms(20);
      break;

    //Send samsung "source" command
    case 2:
      sp_send();
      replyBuf[0] = 0xfe;
      len = 1;
      break;

    //Send samsung command
    case 3:
      //sp_send_code(pgm_read_byte(&sp_codes[0]));
      replyBuf[0] = 0xfe;
      replyBuf[1] = rq->wValue.bytes[0];
      len = 2;
      break;

    // Warm up mode...
    case 4:
      sp_warm_up();
      replyBuf[0] = 0xfe;
      len=1;
      break;

    case 5:
      initPWM();
      replyBuf[0] = 0xfc;
	  len=1;
      break;
  }

  return len;
}

int8_t cnt;
int8_t word;
int8_t pos;
int8_t end;

ISR(TIMER2_COMP_vect)
{
	cnt++;
	if(cnt == 6)
	{
		TCCR2 &= ~(1 << COM20);
	}

	if(cnt == end)
	{
		if( (word  & (1<<pos)) == 0)
		{
			end = 12;
		}
		else
		{
			end = 23;
		}
		pos++;
		if(pos < 8)
		{
			TCCR2 |= (1 << COM20);
		}
		else
		{
			TCCR2 &= ~(1 << CS21);
		}

		cnt = 0;
	}
}

void initPWM()
{
	cnt = 0;
	pos = 0;
	end = 92;
	word = 0b11001010;
	TCCR2 = (1 << WGM21) | (1 << COM20) | (1 << CS21);

	TIMSK |= (1<<OCIE2);
	OCR2 = 209;

	DDRB |= (1 << PB3);
    //DDRC |= (1 << PC1);
}

int main()
{
  wdt_enable(WDTO_1S);
  uint8_t i;
  usbInit();
  sp_init();
  usbDeviceDisconnect();

  i=0;
  while(--i)
  {
    wdt_reset();
    _delay_ms(1);
  }

  usbDeviceConnect();
  sei();

  while(1)
  {
    wdt_reset();
    usbPoll();
  }

  return 0;
}
