#ifndef SERVER_H
#define SERVER_H

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


#endif