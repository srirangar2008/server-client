#ifndef UTILS_H
#define UTILS_H

enum Request
{
    REQUEST = 0,
    QUERY_SERVER_INFO = 1
};

struct datapacket
{
    int id;
    enum Request request;
};

char* currenthostname();
char* getIPAddress();

#endif

