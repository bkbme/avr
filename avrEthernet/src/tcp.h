/*
 * tcp.h
 *
 *  Created on: Sep 28, 2010
 *      Author: berni
 */

#ifndef TCP_H_
#define TCP_H_

typedef struct
{
	uint16_t source_port;
	uint16_t destination_port;
	uint32_t sequence_number;
	uint32_t acknowledgment_number;
	uint8_t  data_offset_reserved;
	uint8_t  flags;
	uint16_t window_size;
	uint16_t checksum;
	uint16_t urgent_pointer;

} tcp_header;

#define TCP_FLAG_CWR	0	//Congestion Window Reduced
#define TCP_FLAG_ECE	1   //ECN-Echo
#define TCP_FLAG_URG	2	//Urgent Pointer is set
#define TCP_FLAG_ACK	3	//ACK is set
#define TCP_FLAG_PSH	4	//Push function
#define TCP_FLAG_RST	5	//Reset connection
#define TCP_FLAG_SYN	6	//Synchronise Sequence Numbers
#define TCP_FLAG_FIN	7	//No more data from Sender

typedef enum
{
	ETS_none=0,
	ETS_listen,
	ETS_SYN_recveived,
	ETS_SYN_sent,
	ETS_Established,
	ETS_Fin_Wait1,
	ETS_Fin_Wait2,
	ETS_Closing,
	ETS_Time_Wait,
	ETS_Close_Wait,
	ETS_Last_ACK,
	ETS_Closed

} EN_Tcp_States;

static EN_Tcp_States tcp_state;

#endif /* TCP_H_ */
