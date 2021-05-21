#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "utils.h"


char* currenthostname()
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
  char* temp_ip;
  host_entry = gethostbyname(currenthostname());
  temp_ip = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
  strncpy(IPAddress, temp_ip, strlen(temp_ip));
  printf("Host IP = %s\n", IPAddress);
  return IPAddress;
}

char* hostName()
{ 
  char* name = (char*)calloc(64,sizeof(char));
  int length = 64;
  FILE* fpipe;
  fpipe = (FILE*)popen("hostname", "r");
  if(fpipe == 0)
  {
    ERROR_LOG("popen failed.\n");
    exit(0);
  }
  fread(name, sizeof(char), length, fpipe);
  printf("%s : %d : The hostname = %s\n", __FILE__, __LINE__, name);
  fclose(fpipe);
  return name;
}

char* ipAddress()
{
  char* ip = (char*)calloc(20, sizeof(char));
  int length = 20;
  FILE* fpipe;
  fpipe = (FILE*)popen("hostname -i","r");
  if(fpipe == 0)
  {
    ERROR_LOG("popen failed.\n");
    exit(0);
  }
  fread(ip, sizeof(char), length, fpipe);
  printf("IP = %s\n", ip);
  fclose(fpipe);
  return ip;
}


