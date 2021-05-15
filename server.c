#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "server.h"
#include "utils.h"

struct datapacket
{
  int id;
  char message[20];
};

int main()
{
	int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char tx_buffer[1024] = {0};
  char rx_buffer[1024] = {0};
  char* server_name = "RaspberryPi : ";
  char* hello = "Hello from the server.";
  struct datapacket data_recvd;

  struct serverInfo serverinfo = 
  {
    .id = 0xFF
  };
  char* serverHostName = currenthostname();
  char* serverIPAddress = getIPAddress();
  strncpy(serverinfo.serverhostname, serverHostName, strlen(serverHostName));
  strncpy(serverinfo.ipAddress, serverIPAddress, strlen(serverIPAddress));
  free(serverHostName);
  free(serverIPAddress);
  exit(0);


  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("Socket creation failed.");
    exit(EXIT_FAILURE);
  }

  if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    perror("setsockopt failed.");
    exit(-1);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(1100);

  if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(-1);
  }
  while(1)
  {
    printf("Waiting for message from client.\n");
  if(listen(server_fd,3) < 0)
  {
    perror("Listen failed");
    exit(-1);
  }

  if((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
  {
    perror("accept failed");
    exit(-1);
  }

  valread = read(new_socket, rx_buffer, sizeof(rx_buffer));
 // valread = read(new_socket, (struct datapacket*)&data_recvd, sizeof(struct datapacket));
  //printf("%s\n", buffer);
 /* if(strlen(&data_recvd.message != 0))
  {
    printf("Server : Message %s received from client.\n");
  }*/
  /*switch(data_recvd.id)
  {
    case 0 : 
        strncpy(buffer, "Option 0", strlen("Option 0"));
        break;
    case 1 : 
        strncpy(buffer, "Option 1", strlen("Option 1"));
        break;
    default : 
        strncpy(buffer, "Option xx", strlen("Option xx"));
        break;
  }*/
  char* sendmessage = (char*)calloc(1024,sizeof(char));
  printf("Message recvd from client.\n");
  printf("%s\n", rx_buffer);
  printf("RaspberryPi : ");
  scanf("%s", sendmessage);
  strncat(tx_buffer, server_name, strlen(server_name));
  strncat(tx_buffer, sendmessage, strlen(sendmessage));
  strncat(tx_buffer, "\n", 1);
  //printf("TX Buffer = %s\n", tx_buffer);
  send(new_socket, tx_buffer, 1024, 0);
  printf("Message sent to client.\n");
  //printf("Server : Option message sent,\n");
  free(sendmessage);
  }
  return 0;
}

