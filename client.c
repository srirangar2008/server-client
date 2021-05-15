#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

struct datapacket
{
  int id;
  char message[20];
};

int main()
{
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char* hello = "Hello from client.\n";
  char buffer[1024] = {0};

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Socket creation failed.\n");
    exit(-1);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(1100);

  if(inet_pton(AF_INET, "192.168.0.116", &serv_addr.sin_addr) <= 0)
  {
    perror("Address not supported.");
    exit(-1);
  }

  if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
  {
    perror("COnnect failed.");
    exit(-1);
  }

  struct datapacket data;
  data.id = 0;
  strncpy(data.message, "Hi server id 0", sizeof(data.message));
  send(sock, (struct datapacket*)&data, sizeof(struct datapacket), 0);
  printf("Hello sent from client.\n");
  valread = read(sock, buffer, 1024);
  printf("Client buffer recvd : %s\n", buffer);
  return 0;
}
