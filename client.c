#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "server.h"
#include "client_serv.h"
#include <ctype.h>

//The main way of communicating is to : 
// Send the datapacket to the server,
// Then expect the response 

void help()
{
  printf("The client is launched. The following are the services provided.\n \
          Option 1 : Register your client.\n \
          Option 2 : Query Server Info.\n \ 
          Option 3 : Terminate.\n");
} 

void parseOptions(char* options, int* optint)
{
  int i = 0 ;
  int j = 0;
  for(i = 0; i < 10; i++)
  {
    printf("options[i] = %x\n", options[i]);
    if(isdigit(options[i]))
    {
      optint[j] = atoi(&options[i]);
      j++;
    }
  }
  for(j = 0; j < i; j++)
  {
    printf("optint[%d] = %d\n", j , optint[j]);
  }
}


int main()
{
  /*char* currentHost = currenthostname();
  char* currentIPAddress = getIPAddress();
  printf("currentHost = %s\n", currentHost);
  printf("Current IP address = %s\n", currentIPAddress);
  free(currentHost);
  free(currentIPAddress);*/
  //exit(0);
  #define MAX_OPTIONS 20
  help();
  printf("Enter the options separated by spaces.\n");
  //exit(0);
  char* options = (char*)calloc(MAX_OPTIONS, sizeof(char));
  int* optint = (int*)calloc(MAX_OPTIONS, sizeof(int));
  int i = 0;
  char* p = options;
  fgets(options, MAX_OPTIONS, stdin);
  /*{ 
    if(*(p+i) == "\n")
      break;
    printf("%d\n",*p);
    ++i;
  }*/
  parseOptions(options, optint);
  printf("options = %s", options);
  //exit(0);
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char* hello = "Hello from client.\n";
  char tx_buffer[1024] = {0};
  char rx_buffer[1024] = {0};
  char* client_name = "RangaGaming3 : ";
  char* host = hostName();
  char* ip = ipAddress();
  

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Socket creation failed.\n");
    exit(-1);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(1100);

  //if(inet_pton(AF_INET, "192.168.0.116", &serv_addr.sin_addr) <= 0)
  if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0)
  {
    perror("Address not supported.");
    exit(-1);
  }
  
  struct datapacket data = { .id = 1, .request = REGISTER};
  char* message = (char*)calloc(1024, sizeof(char));
  if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
  {
    perror("COnnect failed.");
    exit(-1);
  }
  
  while(optint[i] != 0)
  {
    switch(optint[i])
    {
      case 1 : 
      optionRegister(sock);
      break;

      case 2 : 
      printf("Need to implement the server query.\n");
      break;

      case 3 : 
      optionTerminate(sock);
      break;

      default : 
      printf("option not supported.\n");
      break;
    }
    ++i;
  }
  
  sleep(1);
  
  
  
return 0;
}
