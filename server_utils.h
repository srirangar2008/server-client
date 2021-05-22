#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

struct RegisterClients* InitHeadNode();
void PrintClients(struct RegisterClients* head);
int checkDuplicate(struct RegisterClients* head, struct registerClient* data);
void AddNode(struct RegisterClients* head, struct registerClient* data);
void clientregister(struct datapacket *data, int socket_id);
void DeleteNode(struct RegisterClients* head, int clientID);
void clientDeregister(struct datapacket* data, int socket_id);

#endif