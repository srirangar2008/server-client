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