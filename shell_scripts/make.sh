#!/bin/bash

server()
{
  gcc -g -o server server.c utils.c
}

client()
{
  gcc -g -o client client.c utils.c 
}

echo "Compiling server"
server
echo "Compiling client"
client

