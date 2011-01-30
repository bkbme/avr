/*
 * samsung_protocol.c
 *
 *  Created on: May 2, 2010
 *      Author: berni
 */

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define HIGH() PORTC |= (1<<PC0)
#define LOW()  PORTC &= ~(1<<PC0)

void sp_init()
{
  DDRC|= (1<<PC0);
}

void one_bit()
{
  int i=0;
  for(; i<21; i++)
  {
    HIGH();
    _delay_us(13);
    LOW();
    _delay_us(13);
  }
  LOW();
  _delay_us(1690);
}

void zero_bit()
{
  int i=0;
  for(; i<21; i++)
  {
    HIGH();
    _delay_us(13);
    LOW();
    _delay_us(13);
  }
  LOW();
  _delay_us(560);
}

void start_bit()
{
  int i=0;
  for(; i<169; i++)
  {
    HIGH();
    _delay_us(12);
    LOW();
    _delay_us(12);
  }
  LOW();
  _delay_us(4500);
}

void sp_manu_code()
{
  //Startbit
  start_bit();
  //Manufacturer code 111000001110
  one_bit();
  one_bit();
  one_bit();
  zero_bit();
  zero_bit();
  zero_bit();
  zero_bit();
  zero_bit();
  one_bit();
  one_bit();
  one_bit();
  zero_bit();
}

void sp_send()
{
  cli();
  sp_manu_code();

  //"Source" data 000010000000011111111
  zero_bit();
  zero_bit();
  zero_bit();
  zero_bit();
  one_bit();
  zero_bit();
  zero_bit();
  zero_bit();
  zero_bit();
  zero_bit();
  zero_bit();
  zero_bit();
  zero_bit();
  one_bit();
  one_bit();
  one_bit();
  one_bit();
  one_bit();
  one_bit();
  one_bit();
  one_bit();
  sei();
}

void sp_send_code(uint8_t code)
{
  uint8_t i=0;

  sp_manu_code();

  for(i=0;i<4;i++)
    zero_bit();

  for(i=7;i>2;i--)
  {
    if( (code & (1<<i)) != 0)
    {
      one_bit();
    }
    else
    {
      zero_bit();
    }
  }

  for(i=0;i<3;i++)
     zero_bit();

  for(i=7;i>2;i--)
  {
    if( (code & (1<<i)) != 0)
    {
      zero_bit();
    }
    else
    {
      one_bit();
    }
  }

  for(i=0;i<4;i++)
    one_bit();

  sp_manu_code();
}

void sp_warm_up()
{
  uint8_t i,j;
  for(j=0; j<5; j++)
  {
    for(i=0; i<32; i++)
      one_bit();
    _delay_ms(10);
  }
}
