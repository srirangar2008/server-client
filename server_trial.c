#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "server.h"
#include "utils.h"
#include "server_utils.h"


extern struct RegisterClients* headNode;
int lengthOfClientList = 0;

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
  struct datapacket datapack;
  struct registerClient clientReg;
  headNode = InitHeadNode();

  struct serverInfo serverinfo = 
  {
    .id = 0xFF
  };
  char* serverHostName = hostName();
  char* serverIPAddress = ipAddress();
  strncpy(serverinfo.serverhostname, serverHostName, strlen(serverHostName));
  strncpy(serverinfo.ipAddress, serverIPAddress, strlen(serverIPAddress));
  free(serverHostName);
  free(serverIPAddress);

  printf("Server ready now.\n");
  //exit(0);


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
  int cont = 0, end = 0;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(1100);

  if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(-1);
  }
 //while(1)
  //{
    printf("Waiting for message from client.\n");
  if(listen(server_fd,3) < 0)
  {
    perror("Listen failed");
    exit(-1);
  }
  printf("Listen succesdful\n");
  if((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
  {
    perror("accept failed");
    exit(-1);
  }
  printf("accept successful.\n");
  
  printf("valread = %d\n", valread);
 while((valread = read(new_socket, &datapack, sizeof(struct datapacket)) != 0 ))
 {
   printf("new socket = %d\n", new_socket);
   //valread = read(new_socket, &datapack, sizeof(struct datapacket));
  if(datapack.request == REQUEST)
  {
    printf("Server : Calling the register function.\n");
    clientregister(&datapack, new_socket);
    cont = 1;
  }
  else if(datapack.request == TERMINATE)
  {
    printf("Server : Deregistering the client with id %d\n", datapack.id);
    clientDeregister(&datapack, new_socket);
  }
  else
  {
    printf("Server : Rejecting connection.\n");
    end = 1;
  }
  if(end) 
    printf("Connection refused as the client is not registered.\n");
  if(cont) 
    printf("Client registration successful.\n");
}
  return 0;
}

