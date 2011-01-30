/*
 * samsung_protocol.h
 *
 *  Created on: May 2, 2010
 *      Author: berni
 */

#ifndef SAMSUNG_PROTOCOL_H_
#define SAMSUNG_PROTOCOL_H_

#include <avr/pgmspace.h>

/* manu code +
 * vorne 4x0 weg; hinten 4x1 weg
 * dann 5 bit code + 3x0 + vorheriger code invertiert
 * also manu code + 4x0 + 5 bit + 3x0 + ~5bit + 4x1
*/

uint8_t sp_codes[] PROGMEM = {
    0b10000000, //source        - 0
    0b01000000, //poweroff      - 1
    0b10001000, //0             - 2
    0b00100000, //1             - 3
    0b10100000, //2             - 4
    0b01100000, //3             - 5
    0b00010000, //4             - 6
    0b10010000, //5             - 7
    0b01010000, //6             - 8
    0b00110000, //7             - 9
    0b10110000, //8             -10
    0b01110000  //9             -11
};


void sp_init();
void sp_send();
void sp_send_code(uint8_t code);
void sp_warm_up();

#endif /* SAMSUNG_PROTOCOL_H_ */
