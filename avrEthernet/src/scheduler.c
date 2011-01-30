

int8_t scheduler_is_arp_packet(buffer_t* buf)
{
	if(buf->len >= 12) {
		if( (buf->data[12] == LO8(ETHERTYPE_ARP)) &&
			(buf->data[13] == HI8(ETHERTYPE_ARP))) {
			return 0;
		} else {
			return -2;
		}
	} else {
		return -1;
	}
}

int8_t scheduler_is_ip_packet(buffer_t* buf)
{
	if(buf->len >= IP_DATA_OFFSET) {
		if( (buf->data[12] == LO8(ETHERTYPE_IP)) &&
			(buf->data[13] == HI8(ETHERTYPE_IP))) {
			return 0;
		} else {
			return -2;
		}
	} else {
		return -1;
	}
}

int8_t scheduler_is_udp_packet(buffer_t* buf)
{
	if(stack_is_ip_packet(buf) >= 0) {
		if(buf->data[IP_OFFSET+9] == IP_PROTOCOL_UDP) {
			return 0;
		} else {
			return -2;
		}
	} else {
		return -1;
	}
}




