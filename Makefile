#Makefile for client and server

CC=gcc
SERVER_SRCS=server.c server_utils.c utils.c
CLIENT_SRCS=client.c utils.c
SERVER_TRIAL_SRCS=server_trial.c server_utils.c utils.c


all : server client server_trial

server: server.o
	$(CC) -o server $(SERVER_SRCS)

client: client.o
	$(CC) -o client $(CLIENT_SRCS)

server_trial : server_trial.o
	$(CC) -o server_trial $(SERVER_TRIAL_SRCS)

clean : 
	rm server client *.o

