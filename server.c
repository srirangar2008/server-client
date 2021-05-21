#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "server.h"
#include "utils.h"

struct RegisterClients
{
  struct registerClient client;
  struct clientStatus status;
  struct RegisterClients* next;
};

struct RegisterClients* headNode;
int lengthOfClientList = 0;
struct RegisterClients* InitHeadNode()
{
  struct RegisterClients* headNode = (struct RegisterClients*)calloc(1, sizeof(struct RegisterClients));
  headNode->next = NULL;
  //populating headnode with server info.
  headNode->client.id = 0xFF;
  strncpy(headNode->client.clienthostname, currenthostname(), sizeof(headNode->client.clienthostname));
  strncpy(headNode->client.ipAddress, getIPAddress(), sizeof(headNode->client.ipAddress));
  headNode->status.id = 0xFF;
  headNode->status.registered = 1;
  return headNode;
}

void PrintClients(struct RegisterClients* head)
{
  struct RegisterClients* temp = head;
  while(temp != NULL)
  {
    printf("Client ID : %d\n \
            Client Hostname : %s\n \
            Client IP = %s\n \
            Client registartion status : %d\n",
            temp->client.id, temp->client.clienthostname, temp->client.ipAddress, temp->status.registered);
    temp = temp->next;
  }
}

int checkDuplicate(struct RegisterClients* head, struct registerClient* data)
{
  struct RegisterClients* temp = head;
  while(temp != NULL)
  {
    if(strncmp(temp->client.ipAddress, data->ipAddress, strlen(data->ipAddress)) == 0)
    {
      printf("The client already exixts.\n");
      return 1;
    }
    temp = temp->next;
  }
  return 0;
}

void AddNode(struct RegisterClients* head, struct registerClient* data)
{
  int ret = checkDuplicate(head, data);
  if(ret)
  {
    return;
  }
  struct RegisterClients* temp = head;
  while(temp->next != NULL)
  {
    temp = temp->next;
  }
  struct RegisterClients* new = (struct RegisterClients*)calloc(1, sizeof(struct RegisterClients));
  new->client.id = data->id;
  strncpy(new->client.clienthostname, data->clienthostname, strlen(data->clienthostname));
  strncpy(new->client.ipAddress,data->ipAddress,strlen(data->ipAddress));
  new->status.id = data->id;
  new->status.registered = 1;
  new->next = NULL;
  temp->next = new;
  printf("Client added\n");
  PrintClients(head);
}

void clientregister(struct datapacket *data, int socket_id)
{ 
  printf("Querying the client info.\n");
  struct datapacket getClientData;
  struct registerClient clientData;
  getClientData.id = 0xFF;
  getClientData.request = QUERY_CLIENT_INFO;
  send(socket_id, &getClientData, sizeof(struct datapacket), 0);
  printf("Waiting for client information.\n");
  int valread = read(socket_id, &clientData, sizeof(struct registerClient));
  AddNode(headNode, &clientData);
  //exit(0);

}

void DeleteNode(struct RegisterClients* head, int clientID)
{
  struct RegisterClients* temp = head;
  struct RegisterClients* prev = temp;
  while(temp != NULL)
  {
    if(temp->status.id == clientID)
    {
      prev->next = temp->next;
      free(temp);
      return;
    }
    else
    {
      prev = temp;
      temp = temp->next;
      
    }
  }
  printf("Nothing found. Some bug.\n");
  return;
}

void clientDeregister(struct datapacket* data, int socket_id)
{
  printf("Deregistering the client with client id : %d\n", data->id);
  printf("Remove client now. \n");
  DeleteNode(headNode, data->id);
}

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
 while(1)
  {
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
 // while(1)
 // {
  valread = read(new_socket, &datapack, sizeof(struct datapacket));
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
  /*char* sendmessage = (char*)calloc(1024,sizeof(char));
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
  free(sendmessage);*/
  if(end) 
    printf("Connection refused as the client is not registered.\n");
  if(cont) 
    printf("Client registration successful.\n");
  }
  return 0;
}

