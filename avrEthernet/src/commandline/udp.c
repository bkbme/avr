#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

int main()
{
  int sockfd;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd < 0) {
    perror("Failed to open socket!");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("192.168.88.1");
  serv_addr.sin_port = htons(6667);

  char* data = "Hallo Welt!";

  int retVal = sendto(sockfd, data, strlen(data), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if(retVal < 0) {
    perror("Sendto");
    return -2;
  }


  return 1;
}
