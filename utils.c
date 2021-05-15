#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utils.h"


char* hostname()
{
  char* currentHostName = (char*)calloc(64, sizeof(char));
  gethostname(currentHostName, 64);
  printf("Current Host Name = %s\n", currentHostName);
  return currentHostName;
}

char* getIPAddress()
{
  char* IPAddress = (char*)calloc(16, sizeof(char));
  struct hostent* host_entry;
  host_entry = gethostbyname(hostname());
  IPAddress = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
  printf("Host IP = %s\n", IPAddress);
  return IPAddress;
}