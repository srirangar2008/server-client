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
  char* temp = (char*)calloc(64,sizeof(char));
  int length = 64;
  FILE* fpipe;
  fpipe = (FILE*)popen("hostname", "r");
  if(fpipe == 0)
  {
    ERROR_LOG("popen failed.\n");
    exit(0);
  }
  fread(temp, sizeof(char), length, fpipe);
  char* name = (char*)calloc(strlen(temp),sizeof(char));
  strncpy(name, temp, strlen(temp) - 1);
  free(temp);
  printf("%s : %d : The hostname = %s\n", __FILE__, __LINE__, name);
  fclose(fpipe);
  return name;
}

char* ipAddress()
{
  int length = 20;
  char* temp  = (char*)calloc(20, sizeof(char));;
  FILE* fpipe;
  fpipe = (FILE*)popen("hostname -i","r");
  if(fpipe == 0)
  {
    ERROR_LOG("popen failed.\n");
    exit(0);
  }
  fread(temp, sizeof(char), length, fpipe);
  char* ip = (char*)calloc(strlen(temp), sizeof(char));
  strncpy(ip, temp, strlen(temp) - 1);
  free(temp);
  //printf("IP = %s, length = %d", ip, strlen(ip));
  fclose(fpipe);
  return ip;
}


