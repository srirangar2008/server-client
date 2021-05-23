#ifndef UTILS_H
#define UTILS_H


#define ERROR_LOG(fmt, ...) \
    do{ fprintf(stderr, fmt __VA_ARGS__);}while(0);



enum Request
{
    REGISTER = 0,
    QUERY_SERVER_INFO = 1,
    QUERY_CLIENT_INFO = 2,
    TERMINATE = 3
};

struct datapacket
{
    int id;
    enum Request request;
};

char* currenthostname();
char* getIPAddress();

char* hostName();
char* ipAddress();

#endif

