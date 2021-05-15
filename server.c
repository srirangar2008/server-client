#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

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
  char buffer[1024] = {0};
  char* hello = "Hello from the server.";
  struct datapacket data_recvd;
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

  valread = read(new_socket, (struct datapacket*)&data_recvd, sizeof(struct datapacket));
  //printf("%s\n", buffer);
 /* if(strlen(&data_recvd.message != 0))
  {
    printf("Server : Message %s received from client.\n");
  }*/
  switch(data_recvd.id)
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
  }

  send(new_socket, buffer, strlen(buffer), 0);
  printf("Server : Option message sent,\n");
  }
  return 0;
}

