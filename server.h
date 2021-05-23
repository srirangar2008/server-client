#ifndef SERVER_H
#define SERVER_H
//This is to indicate local host.
//#define SERVER_IP "127.0.0.1"

#define SERVER_IP "192.168.0.109"

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
