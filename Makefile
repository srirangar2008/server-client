#Makefile for client and server

CC=gcc
SERVER_SRCS=server.c server_utils.c utils.c
CLIENT_SRCS=client.c utils.c


all : server client 

server: server.o
	$(CC) -o server $(SERVER_SRCS)

client: client.o
	$(CC) -o client $(CLIENT_SRCS)

clean : 
	rm server client *.o

