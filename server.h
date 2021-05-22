#ifndef SERVER_H
#define SERVER_H

#define SERVER_IP "127.0.0.1"
struct registerClient
{
    int id;
    char clienthostname[64];
    char ipAddress[20];
};

struct clientStatus
{
    int registered;
    int id;
};

struct serverInfo
{
    int id;
    char serverhostname[64];
    char ipAddress[20];
};

struct RegisterClients
{
  struct registerClient client;
  struct clientStatus status;
  struct RegisterClients* next;
};

struct RegisterClients* headNode;



#endif
