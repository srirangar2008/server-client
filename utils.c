#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"

char* hostname()
{
  char* currentHostName = (char*)calloc(64, sizeof(char));
  gethostname(currentHostName, 64);
  printf("Current Host Name = %s\n", currentHostName);
  return currentHostName;
}

