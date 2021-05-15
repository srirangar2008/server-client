#!/bin/bash

server()
{
  gcc -o server server.c
}

client()
{
  gcc -o client client.c  
}

echo "Compiling server"
server
echo "Compiling client"
client

