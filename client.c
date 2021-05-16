#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "server.h"


int main()
{
  char* currentHost = currenthostname();
  char* currentIPAddress = getIPAddress();
  printf("currentHost = %s\n", currentHost);
  printf("Current IP address = %s\n", currentIPAddress);
  free(currentHost);
  free(currentIPAddress);
  //exit(0);
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char* hello = "Hello from client.\n";
  char tx_buffer[1024] = {0};
  char rx_buffer[1024] = {0};
  char* client_name = "RangaGaming3 : ";

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Socket creation failed.\n");
    exit(-1);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(1100);

  //if(inet_pton(AF_INET, "192.168.0.116", &serv_addr.sin_addr) <= 0)
  if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0)
  {
    perror("Address not supported.");
    exit(-1);
  }

  if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
  {
    perror("COnnect failed.");
    exit(-1);
  }

  /*struct datapacket data;
  data.id = 0;
  strncpy(data.message, "Hi server id 0", sizeof(data.message));
  */
  struct datapacket data = { .id = 1, .request = REQUEST};
  char* message = (char*)calloc(1024, sizeof(char));
  while(1) 
  {
    //valread = read(sock, rx_buffer, 1024);
    //printf("%s", rx_buffer);
    /*printf("Type your message.\n");
    char* message = (char*)calloc(1024, sizeof(char));
    printf("RangaGaming3 : ");
    bzero(message, sizeof(message));
    scanf("%s",message);
    strncat(tx_buffer, client_name, strlen(client_name));
    strncat(tx_buffer, message, strlen(message));*/
    //printf("TX Buffer = %s", tx_buffer);
    send(sock, &data, sizeof(struct datapacket), 0);
    printf("Message sent.\n");
    //exit(0);
    //printf("Hello sent from client.\n");
    valread = read(sock, &data, sizeof(struct datapacket));
    printf("%d", data.id);
    printf("Message recvd.\n");
    if(data.request == QUERY_CLIENT_INFO)
    {
      struct registerClient* reg = (struct registerClient*)calloc(1, sizeof(struct registerClient));
      strncpy(reg->clienthostname, currenthostname(), sizeof(reg->clienthostname));
      strncpy(reg->ipAddress, getIPAddress(), sizeof(reg->ipAddress));
      reg->id = 1;
      send(sock, reg, sizeof(struct registerClient), 0);
      printf("Sent the client info.\n");

    }
    sleep(1);
    printf("Client terminating.\n");
    data.id = 1;
    data.request = TERMINATE;
    //send(sock, &data, sizeof(struct datapacket), 0);
    //printf("Client buffer recvd : %s\n", buffer);
    //bzero(message, sizeof(message));
    free(message);
    break;
  }
  
  return 0;
}
