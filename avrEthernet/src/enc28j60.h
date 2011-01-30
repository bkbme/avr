/*
 * enc28j60.h
 *
 *  Created on: Jul 14, 2010
 *      Author: berni
 */

#ifndef ENC28J60_H_
#define ENC28J60_H_

#include "ethernet.h"

#define ENC_ENABLE_DEBUG

#define RXTX_BUFMEM_BORDER 0x5de

#define rdef(bank,addr,ismac) 	((bank<<6)|(ismac<<5)|(addr&0x1f))
#define bmsk(addr)				(addr>>6)
#define adrmsk(addr)			(addr&0x1f)
#define mamsk(addr)				((addr>>5)&0x1)

/*Banks:
 * 0-3 	Control Registers
 * 4	Phy Registers
 */

//Dont care about the current bank:
#define ENC_EIE 		rdef(0,0x1b,0)
#define ENC_EIR 		rdef(0,0x1c,0)
#define ENC_ESTAT 		rdef(0,0x1d,0)
#define ENC_ECON2 		rdef(0,0x1e,0)
#define ENC_ECON1 		rdef(0,0x1f,0)

//Bank0
#define ENC_ERDPTL 		rdef(0,0x0,0)
#define ENC_ERDPTH		rdef(0,0x1,0)
#define ENC_EWRPTL 		rdef(0,0x2,0)
#define ENC_EWRPTH 		rdef(0,0x3,0)
#define ENC_ETXSTL 		rdef(0,0x4,0)
#define ENC_ETXSTH 		rdef(0,0x5,0)
#define ENC_ETXNDL 		rdef(0,0x6,0)
#define ENC_ETXNDH 		rdef(0,0x7,0)
#define ENC_ERXSTL 		rdef(0,0x8,0)
#define ENC_ERXSTH 		rdef(0,0x9,0)
#define ENC_ERXNDL 		rdef(0,0xA,0)
#define ENC_ERXNDH 		rdef(0,0xB,0)
#define ENC_ERXRDPTL 	rdef(0,0xC,0)
#define ENC_ERXRDPTH 	rdef(0,0xD,0)
#define ENC_ERXWRPTL 	rdef(0,0xE,0)
#define ENC_ERXWRPTH 	rdef(0,0xF,0)
#define ENC_EDMASTL 	rdef(0,0x10,0)
#define ENC_EDMASTH 	rdef(0,0x11,0)
#define ENC_EDMANDL 	rdef(0,0x12,0)
#define ENC_EDMANDH 	rdef(0,0x13,0)
#define ENC_EDMADSTL	rdef(0,0x14,0)
#define ENC_EDMADSTH 	rdef(0,0x15,0)
#define ENC_EDMACSL 	rdef(0,0x16,0)
#define ENC_EDMACSH 	rdef(0,0x17,0)

//Bank1
#define ENC_EHT0 		rdef(1,0x0,0)
#define ENC_EHT1		rdef(1,0x1,0)
#define ENC_EHT2 		rdef(1,0x2,0)
#define ENC_EHT3 		rdef(1,0x3,0)
#define ENC_EHT4 		rdef(1,0x4,0)
#define ENC_EHT5 		rdef(1,0x5,0)
#define ENC_EHT6 		rdef(1,0x6,0)
#define ENC_EHT7 		rdef(1,0x7,0)
#define ENC_EPMM0 		rdef(1,0x8,0)
#define ENC_EPMM1 		rdef(1,0x9,0)
#define ENC_EPMM2 		rdef(1,0xA,0)
#define ENC_EPMM3 		rdef(1,0xB,0)
#define ENC_EPMM4 		rdef(1,0xC,0)
#define ENC_EPMM5 		rdef(1,0xD,0)
#define ENC_EPMM6 		rdef(1,0xE,0)
#define ENC_EPMM7 		rdef(1,0xF,0)
#define ENC_EPMCSL	 	rdef(1,0x10,0)
#define ENC_EPMCSH		rdef(1,0x11,0)
#define ENC_EPMOL		rdef(1,0x14,0)
#define ENC_EPMOH 		rdef(1,0x15,0)
#define ENC_ERXFCON		rdef(1,0x18,0)
#define ENC_EPKTCNT 	rdef(1,0x19,0)

//Bank2
#define ENC_MACON1 		rdef(2,0x0,1)
#define ENC_MACON3 		rdef(2,0x2,1)
#define ENC_MACON4 		rdef(2,0x3,1)
#define ENC_MABBIPG		rdef(2,0x4,1)
#define ENC_MAIPGL 		rdef(2,0x6,1)
#define ENC_MAIPGH 		rdef(2,0x7,1)
#define ENC_MACLCON1	rdef(2,0x8,1)
#define ENC_MACLCON2	rdef(2,0x9,1)
#define ENC_MAMXFLL		rdef(2,0xA,1)
#define ENC_MAMXFLH		rdef(2,0xB,1)
#define ENC_MICMD 		rdef(2,0x12,1)
#define ENC_MIREGADR	rdef(2,0x14,1)
#define ENC_MIWRL	 	rdef(2,0x16,1)
#define ENC_MIWRH	 	rdef(2,0x17,1)
#define ENC_MIRDL 		rdef(2,0x18,1)
#define ENC_MIRDH 		rdef(2,0x19,1)

//Bank3
#define ENC_MAADR5 		rdef(3,0x0,1)
#define ENC_MAADR6		rdef(3,0x1,1)
#define ENC_MAADR3 		rdef(3,0x2,1)
#define ENC_MAADR4 		rdef(3,0x3,1)
#define ENC_MAADR1 		rdef(3,0x4,1)
#define ENC_MAADR2 		rdef(3,0x5,1)
#define ENC_EBSTSD 		rdef(3,0x6,0)
#define ENC_EBSTCON		rdef(3,0x7,0)
#define ENC_EBSTCSL		rdef(3,0x8,0)
#define ENC_EBSTCSH		rdef(3,0x9,0)
#define ENC_MISTAT 		rdef(3,0xA,1)
#define ENC_EREVID	 	rdef(3,0x12,0)
#define ENC_ECOCON	 	rdef(3,0x15,0)
#define ENC_EFLOCON	 	rdef(3,0x17,0)
#define ENC_EPAUSL 		rdef(3,0x18,0)
#define ENC_EPAUSH 		rdef(3,0x19,0)
//ENC REG END

//Important Bitfields

#define ENC_EIE_INTIE	7
#define ENC_EIE_PKTIE	6
#define ENC_EIE_DMAIE	5
#define ENC_EIE_LINKIE	4
#define ENC_EIE_TXIE	3
#define ENC_EIE_TXERIE	1
#define ENC_EIE_RXERIE	0

#define ENC_EIR_PKTIF	6
#define ENC_EIR_DMAIF	5
#define ENC_EIR_LINKIF	4
#define ENC_EIR_TXIF	3
#define ENC_EIR_TXERIF	1
#define ENC_EIR_RXERIF	0

#define ENC_ESTAT_INT		7
#define ENC_ESTAT_BUFER		6
#define ENC_ESTAT_LATECOL	4
#define ENC_ESTAT_RXBUSY	2
#define ENC_ESTAT_TXABRT	1
#define ENC_ESTAT_CLKRDY	0

#define ENC_ECON2_AUTOINC	7
#define ENC_ECON2_PKTDEC	6
#define ENC_ECON2_PWRSV		5
#define ENC_ECON2_VRPS		3

#define ENC_ECON1_TXRST		7
#define ENC_ECON1_RXRST		6
#define ENC_ECON1_DMAST		5
#define ENC_ECON1_CSUMEN	4
#define ENC_ECON1_TXRTS		3
#define ENC_ECON1_RXEN		2
#define ENC_ECON1_BSEL1		1
#define ENC_ECON1_BSEL0		0

#define ENC_ERXFCON_UCEN	7
#define ENC_ERXFCON_ANDOR	6
#define ENC_ERXFCON_CRCEN	5
#define ENC_ERXFCON_PMEN	4
#define ENC_ERXFCON_MPEN	3
#define ENC_ERXFCON_HTEN	2
#define ENC_ERXFCON_MCEN	1
#define ENC_ERXFCON_BCEN	0

#define ENC_MACON1_TXPAUS	3
#define ENC_MACON1_RXPAUS	2
#define ENC_MACON1_PASSALL	1
#define ENC_MACON1_MARXEN	0

#define ENC_MACON3_PADCFG2	7
#define ENC_MACON3_PADCFG1	6
#define ENC_MACON3_PADCFG0	5
#define ENC_MACON3_TXCRCEN	4
#define ENC_MACON3_PHDREN	3
#define ENC_MACON3_HFRMEN	2
#define ENC_MACON3_FRMLNEN	1
#define ENC_MACON3_FULDPX	0

#define ENC_MACON4_DEFER	6
#define ENC_MACON4_BPEN		5
#define ENC_MACON4_NOBKOFF	4

#define ENC_MICMD_MIISCAN	1
#define ENC_MICMD_MIIRD		0

#define ENC_MISTAT_NVALID	2
#define ENC_MISTAT_SCAN		1
#define ENC_MISTAT_BUSY		0


//PHY Registers
#define ENC_PHCON1			rdef(0,0x00,0)
#define ENC_PHSTAT1			rdef(0,0x01,0)
#define ENC_PHID1			rdef(0,0x02,0)
#define ENC_PHID2			rdef(0,0x03,0)
#define ENC_PHCON2			rdef(0,0x10,0)
#define ENC_PHSTAT2			rdef(0,0x11,0)
#define ENC_PHIE			rdef(0,0x12,0)
#define ENC_PHIR			rdef(0,0x13,0)
#define ENC_PHLCON			rdef(0,0x14,0)

//PHY Register bitfields
#define ENC_PHCON1_PRST		15
#define ENC_PHCON1_PLOOPBK	14
#define ENC_PHCON1_PPWRSV	11
#define ENC_PHCON1_PDPXMD	8

#define ENC_PHSTAT1_PFDPX	12
#define ENC_PHSTAT1_PHDPX	11
#define ENC_PHSTAT1_LLSTAT	2
#define ENC_PHSTAT1_JBSTAT	1

#define ENC_PHCON2_FRCLNK	14
#define ENC_PHCON2_TXDIS	13
#define ENC_PHCON2_JABBER	10
#define ENC_PHCON2_HDLDIS	8

typedef union
{
	uint8_t status[7];
	struct
	{
		uint16_t transmitted_length;
		uint8_t  stat1;
		uint8_t  stat2;
		uint16_t total_bytes_on_wire;
		uint8_t  stat3;

	} content;

} enc_status_vector;

typedef union
{
	uint8_t data[4];
	struct
	{
		uint16_t transmitted_length;
		uint16_t status;
	} content;
} enc_receive_status_vector;

/*Receive Status Vector*/
#define ENC_RXS_LDE 	0  //Long Event / Drop Event
#define ENC_RXS_CE  	2  //Carrier Event previously seen
#define ENC_RXS_CRCE	4  //CRC Error
#define ENC_RXS_LCE		5  //Length Check Error
#define ENC_RXS_LOR		6  //Length out of Range (>1500)
#define ENC_RXS_OK		7  //RX Ok
#define ENC_RXS_RMP		8  //RX Multicast Paket
#define ENC_RXS_RBP		9  //RX Broadcast Paket
#define ENC_RXS_DN		10 //Dribble Nibble ocurred
#define ENC_RXS_CF		11 //RX Control Frame
#define ENC_RXS_PCF		12 //RX Pause Control Frame
#define ENC_RXS_UOP		13 //RX Unknown Opcode
#define ENC_RXS_VL		14 //RX VLAN Tagged Frame

uint8_t enc_rcr(uint8_t addr);
void 	enc_wcr(uint8_t addr, uint8_t data);
void 	enc_wbm(uint16_t addr, uint8_t data);
uint8_t enc_rbm(uint16_t addr);
void 	enc_bfs(uint8_t addr, uint8_t data);
void 	enc_bfc(uint8_t addr, uint8_t data);
void 	enc_reset();
void 	enc_switch_bank(uint8_t bank);
void 	enc_init();

void	 	enc_add_packet_data(uint8_t* data, uint16_t len);
void 		enc_prepare_ethernet_packet(ethernet_header* eth);
uint8_t 	enc_send_packet();
int8_t 		enc_recv_packet();
uint16_t 	enc_rpr(uint8_t addr);
void 		enc_wpr(uint8_t addr, uint16_t data);
uint16_t 	enc_crc(uint16_t mem_adr_start, uint16_t mem_adr_stop);

#endif /* ENC28J60_H_ */
