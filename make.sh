#!/bin/bash

server()
{
  gcc -o server server.c utils.c
}

client()
{
  gcc -o client client.c utils.c 
}

echo "Compiling server"
server
echo "Compiling client"
client

