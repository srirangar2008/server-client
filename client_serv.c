#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "server.h"
#include "client_serv.h"

#define CLIENT_ID 2 


void optionRegister(int socket_id)
{
    int valread;
    struct datapacket data = {.id = CLIENT_ID, .request = REGISTER };
    send(socket_id, &data, sizeof(struct datapacket), 0);
    printf("Message sent.\n");
    valread = read(socket_id, &data, sizeof(struct datapacket));
    
    if(data.request == QUERY_CLIENT_INFO)
    {
      struct registerClient* reg = (struct registerClient*)calloc(1, sizeof(struct registerClient));
      strncpy(reg->clienthostname, hostName(), sizeof(reg->clienthostname));
      strncpy(reg->ipAddress, ipAddress(), sizeof(reg->ipAddress));
      reg->id = CLIENT_ID;
      send(socket_id, reg, sizeof(struct registerClient), 0);
      printf("Sent the client info.\n");
    }
    return;
}

void optionTerminate(int socket_id)
{
    struct datapacket data = {.id = CLIENT_ID, .request = TERMINATE};
    send(socket_id, &data, sizeof(struct datapacket), 0);
    printf("Message sent.\n");
}
