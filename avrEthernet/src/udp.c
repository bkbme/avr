/*
 * udp.c
 *
 *  Created on: Sep 28, 2010
 *      Author: berni
 */
#include "udp.h"
#include "ip.h"
#include "ethernet.h"
#include "enc28j60.h"
#include "string.h"
#include "buffer.h"
#include <stdio.h>

/**Generates an ethernet + ip + udp header.
 * @param dest_addr: ip destination address
 * @param src_port: udp source port number
 * @param dest_port: udp destination port number
 * @param dest: destination memory in which the header should be written.
 */
void udp_gen_header(ip_addr* dest_addr, uint16_t* src_port, uint16_t* dest_port, uint8_t* dest)
{
	printf("udp_gen_header 0\n");
	ip_gen_header(dest_addr, IP_PROTOCOL_UDP, dest);

	printf("udp_gen_header 1\n");
	udp_header* udp = (udp_header*)(dest+UDP_OFFSET);
	udp->destination_port = HTONS(*dest_port);
	udp->source_port = HTONS(*src_port);
	printf("udp_gen_header 3\n");
}

/**Adds data to the udp packet. Does the checksum calculations (ip + udp)
 * and finally sends the packet.
 * @param data: Data that should be added to the udp packet.
 * @param len: Length of the data.
 * @param dest: destination memory in which the header should be written.
 */
void udp_set_data(uint8_t* data, uint16_t len, uint8_t* dest)
{
	printf("udp_set_data 0\n");
	memcpy(dest+UDP_DATA_OFFSET, data, len);
	enc_prepare_ethernet_packet((ethernet_header*)dest);
	ip_header* ip = (ip_header*)(dest+IP_OFFSET);
	udp_header* udp = (udp_header*)(dest+UDP_OFFSET);
	udp->length=HTONS(8+len);
	ip->total_length = 28 + len;
	printf("udp_set_data 1\n");

	enc_add_packet_data(dest+IP_OFFSET, IP_HLEN+UDP_HLEN+len);

	printf("udp_set_data 2\n");
	uint16_t crc = enc_crc(IP_OFFSET, IP_OFFSET+IP_HLEN-1);
	enc_wbm(IP_OFFSET_CHECKSUM_HI8, HI8(crc));
	enc_wbm(IP_OFFSET_CHECKSUM_LO8, LO8(crc));

	crc = enc_crc(UDP_OFFSET, UDP_OFFSET + UDP_DATA_OFFSET + len);
	enc_wbm(UDP_OFFSET_CHECKSUM_HI8, HI8(crc));
	enc_wbm(UDP_OFFSET_CHECKSUM_LO8, LO8(crc));

	printf("udp_set_data 3\n");

	enc_send_packet();
	printf("udp_set_data 4\n");
}

/**Sends a UDP packet.
 * @param data: Data for the udp packet.
 * @param len: Length of the data for the udp packet.
 * @param ip_dest: Destination ip address.
 * @param src_port: UDP source port number.
 * @param dest_port: UDP destination port number.
 */
void udp_send_packet(uint8_t* data, uint16_t len, ip_addr* ip_dest, uint16_t src_port, uint16_t dest_port)
{
	buffer_t* buf = NULL;
	buffer_create(buf, UDP_PLEN+len);
	udp_gen_header(ip_dest, &src_port, &dest_port, buf->data);
	udp_set_data(data, len, buf->data);
}
