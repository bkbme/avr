#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <string.h>
#include <stdlib.h>

#define ETH_FR_LEN 500

int main(int argc, char* argv[])
{
  int s;
  
  s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if(s == -1)
    perror("socket");


  struct sockaddr_ll socket_address;
  void* buffer = (void*)malloc(ETH_FR_LEN);

  unsigned char* etherhead = buffer;
  unsigned char* data = buffer + 14;
  struct ethhdr *eh = (struct ethhdr*) etherhead;

  int send_result = 0;

  unsigned char src_mac[6] = {0x00, 0x16, 0x41, 0x11, 0x2c, 0xa2};
  unsigned char dest_mac[6] = {0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x01};

  socket_address.sll_family = PF_PACKET;
  socket_address.sll_protocol = htons(ETH_P_IP);

  socket_address.sll_ifindex = 2;

  socket_address.sll_hatype = ARPHRD_ETHER;
  socket_address.sll_pkttype = PACKET_OTHERHOST;
  socket_address.sll_halen = ETH_ALEN;

  socket_address.sll_addr[0] = 0x00;
  socket_address.sll_addr[1] = 0x04;
  socket_address.sll_addr[2] = 0x75;
  socket_address.sll_addr[3] = 0xC8;
  socket_address.sll_addr[4] = 0x28;
  socket_address.sll_addr[5] = 0xE5;
  socket_address.sll_addr[6] = 0x00;
  socket_address.sll_addr[7] = 0x00;

  memcpy((void*)buffer, (void*)dest_mac, ETH_ALEN);
  memcpy((void*)(buffer+ETH_ALEN), (void*)src_mac, ETH_ALEN);
  //unsigned short len = 500;
  eh->h_proto = htons(500-18);

  int i;
  for(i=0; i<500-18; i++)
  {
    data[i] = i%255;
  }

  send_result = sendto(s, buffer, ETH_FR_LEN, 0, 
      (struct sockaddr*)&socket_address, sizeof(socket_address));

  if(send_result == -1)
    perror("sendto");


  return 0;
}
