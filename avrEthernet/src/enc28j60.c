/*
 * enc28j60.c
 *
 *  Created on: Jul 14, 2010
 *      Author: berni
 */

#include "spi.h"
#include "enc28j60.h"
#include <util/delay.h>
#include "uart.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include "bit-macros.h"
#include "debug.h"
#include "buffer.h"
#include "kernel.h"

static uint16_t	m_enc_mem_addr;
static uint8_t  m_enc_current_bank;

extern kernel_packet_buffer_t	kernel_packet_buffer;

/**Read Control Register.
 * @param addr: address of the register to read.
 */
uint8_t enc_rcr(uint8_t addr)
{
	uint8_t retVal;

	uint8_t adr = adrmsk(addr);
	if(adr < 0x1b)	{
		enc_switch_bank(bmsk(addr));
	}

	CS_LOW

	spi_write(adr);
	retVal = spi_write(0);
	//If MAC or MI Reg -> dummy byte -> Read twice
	if(mamsk(addr)) {
		retVal = spi_write(0);
	}

	CS_HIGH

	return retVal;
}

/**Read Phy register.
 *
 */
uint16_t enc_rpr(uint8_t addr)
{
	enc_wcr(ENC_MIREGADR, adrmsk(addr));
	enc_bfs(ENC_MICMD, 1<<ENC_MICMD_MIIRD);
	_delay_us(11);
	while(cbs(enc_rcr(ENC_MISTAT), ENC_MISTAT_BUSY));
	enc_bfc(ENC_MICMD, 1<<ENC_MICMD_MIIRD);
	return (enc_rcr(ENC_MIRDL) | (enc_rcr(ENC_MIRDH) << 8));
}

/**Write Phy register.
 *
 */
void enc_wpr(uint8_t addr, uint16_t data)
{
	enc_wcr(ENC_MIREGADR, adrmsk(addr));
	enc_wcr(ENC_MIWRL, data & 0xff);
	enc_wcr(ENC_MIWRH, (data & 0xff00) >> 8);

	_delay_us(11);
	while(cbs(enc_rcr(ENC_MISTAT), ENC_MISTAT_BUSY));
}

/**Write Control Register.
 * @param addr: Address of the register to write.
 * @param data: Data to write into the register.
 */
void enc_wcr(uint8_t addr, uint8_t data)
{
	uint8_t adr = adrmsk(addr);
	if(adr < 0x1b)	{
		enc_switch_bank(bmsk(addr));
	}

	CS_LOW
	spi_write( (0b010 << 5) | (adr) );
	spi_write(data);
	CS_HIGH
}

/**Read Buffer Memory.
 * @param addr: the memory address to read.
 */
uint8_t enc_rbm(uint16_t addr)
{
	uint8_t retVal;

	enc_wcr(ENC_ERDPTL, addr & 0x00ff);
	enc_wcr(ENC_ERDPTH, (addr & 0xff00) >> 8);

	CS_LOW
	spi_write(0b00111010);
	retVal = spi_write(0);
	CS_HIGH

	return retVal;
}

/**Write Buffer Memory
 * @param addr: Memory address to write to.
 * @param data: Data to write into mem addr.
 */
void enc_wbm(uint16_t addr, uint8_t data)
{
	enc_wcr(ENC_EWRPTL, addr & 0x00ff);
	enc_wcr(ENC_EWRPTH, (addr & 0xff00) >> 8);

	CS_LOW
	spi_write( 0b01111010 );
	spi_write(data);
	CS_HIGH
}

/**Bit field set.
 * @param addr: Address of the register that should be set.
 * @param data: Data that is ORed with data in register at addr.
 */
void enc_bfs(uint8_t addr, uint8_t data)
{
	uint8_t adr = adrmsk(addr);
	if(adr < 0x1f) {
		enc_switch_bank(bmsk(addr));
	}

	CS_LOW
	spi_write( (0b100 << 5) | (adr) );
	spi_write(data);
	CS_HIGH
}

/**Bit field clear.
 * @param addr:
 * @param data:
 */
void enc_bfc(uint8_t addr, uint8_t data)
{
	uint8_t adr = adrmsk(addr);
	if(adr < 0x1f) {
		enc_switch_bank(bmsk(addr));
	}

	CS_LOW
	spi_write( (0b101 << 5) | (adr) );
	spi_write(data);
	CS_HIGH
}

/**Reset the enc28j60.
 *
 */
void enc_reset()
{
	CS_LOW
	spi_write(0xff);
	CS_HIGH
}

/**Switch the register bank.
 *
 */
inline void enc_switch_bank(uint8_t bank)
{
	if(m_enc_current_bank != bank)
	{
		enc_bfc(ENC_ECON1, 0x03 << ENC_ECON1_BSEL0);
		enc_bfs(ENC_ECON1, bank & 0x03);
		m_enc_current_bank = bank;
	}
}

/**Init the enc28j60.
 *
 */
void enc_init()
{
	//Enable interrupt on pin INT2
	EIMSK |= 1<<INT2;
	//Raise interrupt on falling edge
	EICRA |= (1<<ISC21);

	m_enc_mem_addr = 0x0;
	m_enc_current_bank = 255;

	while(!cbs(enc_rcr(ENC_ESTAT), ENC_ESTAT_CLKRDY)) {
			_delay_ms(100);
	}

	enc_reset();
	_delay_ms(5);

	enc_bfc(ENC_ECON2, 1<<ENC_ECON2_AUTOINC);

	enc_bfs(ENC_EIE, (1<<ENC_EIE_INTIE) | (1<<ENC_EIE_PKTIE));

	enc_wcr(ENC_ERXNDL, 0xff);
	enc_wcr(ENC_ERXNDH, 0x1f);
	enc_wcr(ENC_ERXSTL, LO8(RXTX_BUFMEM_BORDER));
	enc_wcr(ENC_ERXSTH, HI8(RXTX_BUFMEM_BORDER));
	enc_wcr(ENC_ERXRDPTL, LO8(RXTX_BUFMEM_BORDER));
	enc_wcr(ENC_ERXRDPTH, HI8(RXTX_BUFMEM_BORDER));
	enc_wcr(ENC_ETXSTL, 0x00);
	enc_wcr(ENC_ETXSTH, 0x00);
	enc_wcr(ENC_ETXNDL, LO8(RXTX_BUFMEM_BORDER)-1);
	enc_wcr(ENC_ETXNDH, HI8(RXTX_BUFMEM_BORDER));
	enc_wcr(ENC_ERDPTL, 0x00);
	enc_wcr(ENC_ERDPTH, 0x00);

	enc_wcr(ENC_MACON1,
			((1<<ENC_MACON1_TXPAUS)|(1<<ENC_MACON1_RXPAUS)|(1<<ENC_MACON1_MARXEN)));
	enc_wcr(ENC_MACON3,
			((1<<ENC_MACON3_PADCFG0)|(1<<ENC_MACON3_TXCRCEN)|
			 (1<<ENC_MACON3_FULDPX) |(1<<ENC_MACON3_FRMLNEN)));

	//Set PDPXMD
	enc_wpr(ENC_PHCON1, enc_rpr(ENC_PHCON1) | (1<<ENC_PHCON1_PDPXMD));

	enc_wcr(ENC_MABBIPG, 0x15);
	enc_wcr(ENC_MAIPGL, 0x12);
	enc_wcr(ENC_MAIPGH, 0x00);
	enc_wcr(ENC_MAMXFLL, LO8(NET_MAX_ETH_FRALEN+4));
	enc_wcr(ENC_MAMXFLH, HI8(NET_MAX_ETH_FRALEN+4));

	enc_wcr(ENC_MAADR1, ENC_MAC_0);
	enc_wcr(ENC_MAADR2, ENC_MAC_1);
	enc_wcr(ENC_MAADR3, ENC_MAC_2);
	enc_wcr(ENC_MAADR4, ENC_MAC_3);
	enc_wcr(ENC_MAADR5, ENC_MAC_4);
	enc_wcr(ENC_MAADR6, ENC_MAC_5);

	//Receiving...
	enc_bfs(ENC_ERXFCON, 1<<ENC_ERXFCON_UCEN);
	enc_bfs(ENC_ECON1, 1<<ENC_ECON1_RXEN);
}

void enc_prepare_ethernet_packet(ethernet_header* eth)
{
	enc_wcr(ENC_ETXSTL, m_enc_mem_addr & 0x00ff);
	enc_wcr(ENC_ETXSTH, (m_enc_mem_addr & 0xff00) >> 8);

	//char controlByte = 0x0e;
	uint8_t controlByte = 0x00;
	enc_add_packet_data(&controlByte, 1);
	enc_add_packet_data((uint8_t*)eth, ETH_HLEN);
}

void enc_add_packet_data(uint8_t* data, uint16_t len)
{
	uint16_t pos=0;
	for(;pos < len; pos++)
	{
		enc_wbm(m_enc_mem_addr++, data[pos]);
	}

#ifdef ENC_ENABLE_DEBUG
	//printf("%s: mbpos is 0x%x\n", __FUNCTION__, m_enc_mem_addr);
#endif //ENC_ENABLE_DEBUG
}

uint8_t enc_send_packet()
{
	m_enc_mem_addr--;
	enc_wcr(ENC_ETXNDL, m_enc_mem_addr & 0x00ff);
	enc_wcr(ENC_ETXNDH, (m_enc_mem_addr & 0xff00) >> 8);

	enc_bfc(ENC_EIR, 1<<ENC_EIR_TXIF);
	enc_bfs(ENC_ECON1, 1<<ENC_ECON1_TXRTS);

	while(cbs(enc_rcr(ENC_ECON1), ENC_ECON1_TXRTS))
	{ }

	m_enc_mem_addr = 0x0;

#ifdef bla // ENC_ENABLE_DEBUG
	//Debug only
	enc_status_vector status;
	uint16_t pos;
	uint8_t i=0;
	for(pos=m_enc_mem_addr+1; pos < m_enc_mem_addr+8; pos++)
	{
		status.status[i++] = enc_rbm(pos);
	}

	printf("Ethernet Packet Transmit status:\n");
	printf("Packet length: 0x%x\n", status.content.transmitted_length);
	printf("Bytes on wire: 0x%x\n", status.content.total_bytes_on_wire);
#endif //ENC_ENABLE_DEBUG

	return 1;
}

int8_t enc_recv_packet()
{
	/*
	if(!cbs(enc_rcr(ENC_EIR),ENC_EIR_PKTIF)) {
		printf("No new packets!\n");
		return -1;
	}
	*/

	uint16_t start = (enc_rcr(ENC_ERXRDPTL) | (enc_rcr(ENC_ERXRDPTH) << 8));
	//uint8_t next_l = enc_rbm(start);
	//uint8_t next_h = enc_rbm(start+1);
	uint16_t next = (enc_rbm(start) | (enc_rbm(start+1) << 8));

	start+=6;
	uint16_t len = next-start;

	buffer_t* buf;
	buffer_create(buf, len);

	uint16_t pos=0;
	for(;pos < len; pos++) {
		buf->data[pos] = enc_rbm(start+pos);
	}

	kernel_packet_buffer.tmp_storage = buf;
	//kernel_add_packet(buf);

	//Advance the read pointer to the next paket
	enc_wcr(ENC_ERXRDPTL, LO8(next));
	enc_wcr(ENC_ERXRDPTH, HI8(next));
	//Decrement the paket counter
	enc_bfs(ENC_ECON2, 1<<ENC_ECON2_PKTDEC);


#ifdef bla //ENC_ENABLE_DEBUG
	enc_receive_status_vector status;
	uint8_t i=0;
	for(;i<4; i++)
	{
		status.data[i] = enc_rbm(start+2+i);
	}

	printf("-----RX Status Vector start:%x len:%x status (%x) ------\n", start,
			status.content.transmitted_length, status.content.status);
	if(cbs(status.content.status, ENC_RXS_LDE))		printf("Long Event\n");
	if(cbs(status.content.status, ENC_RXS_CE))		printf("Carrier Event seen\n");
	if(cbs(status.content.status, ENC_RXS_CRCE))	printf("CRC Error\n");
	if(cbs(status.content.status, ENC_RXS_LCE))		printf("Length Check Error\n");
	if(cbs(status.content.status, ENC_RXS_LOR))		printf("Length out of Range\n");
	if(cbs(status.content.status, ENC_RXS_OK))		printf("RX Ok\n");
	if(cbs(status.content.status, ENC_RXS_RMP))		printf("Multicast Packet\n");
	if(cbs(status.content.status, ENC_RXS_RBP))		printf("Broadcast Packet\n");
	if(cbs(status.content.status, ENC_RXS_DN))		printf("Dribble Nibble\n");
	if(cbs(status.content.status, ENC_RXS_CF))		printf("Control Frame\n");
	if(cbs(status.content.status, ENC_RXS_PCF))		printf("Pause Control Frame\n");
	if(cbs(status.content.status, ENC_RXS_UOP))		printf("Unkown Opcode\n");
	if(cbs(status.content.status, ENC_RXS_VL))		printf("Vlan Tagged\n");
	printf("---------------------------\n");

	start+=6;
	uint16_t len = next-start;

	uint8_t data[1500];
	uint16_t pos=0;
	for(;pos < len; pos++) {
		data[pos] = enc_rbm(start+pos);
	}

	debug_dump_mem(data, len);
#endif //ENC_ENABLE_DEBUG

	return 1;
}

/**Calculates crc checksum with the enc hardware engine.
 * Watch out!: the start and end adresses are included!
 * Normally you might want to subtract 1 from the stop adr.
 * @param mem_adr_start: enc bufmem start included in crc
 * @param mem_adr_stop: enc bufmem end included in crc
 * @return calculated crc value.
 */
uint16_t enc_crc(uint16_t mem_adr_start, uint16_t mem_adr_stop)
{
	mem_adr_start++;
	mem_adr_stop++;
	enc_wcr(ENC_EDMASTL, LO8(mem_adr_start));
	enc_wcr(ENC_EDMASTH, HI8(mem_adr_start));
	enc_wcr(ENC_EDMANDL, LO8(mem_adr_stop));
	enc_wcr(ENC_EDMANDH, HI8(mem_adr_stop));

	//Place intterrupt enable here
	enc_bfs(ENC_ECON1, (1<<ENC_ECON1_CSUMEN) | (1<<ENC_ECON1_DMAST));

	//Wait 'til finished
	while(cbs(enc_rcr(ENC_ECON1), 1<<ENC_ECON1_DMAST));

	return (enc_rcr(ENC_EDMACSL) | (enc_rcr(ENC_EDMACSH) << 8));
}

ISR(INT2_vect)
{
	//First disable interrupts
	enc_bfc(ENC_EIE, 1<<ENC_EIE_INTIE);

	uint8_t eir = enc_rcr(ENC_EIR);
	if(cbs(eir, ENC_EIR_PKTIF))
	{
		enc_recv_packet();
	}
	else
	{
		printf("unimp interrupt!\n");
	}

	//Last enable interrupts
	enc_bfs(ENC_EIE, 1<<ENC_EIE_INTIE);
}


