#ifndef SOCKETUTILS_H
#define SOCKETUTILS_H

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/types.h>
#include<string.h>
#include<malloc.h>
#include<unistd.h>
#include<stdbool.h>
#include<pthread.h>


struct getSocket {
	int acceptedClientSocket;
	struct sockaddr_in addr;
	int error;
	bool accepted;
};


struct sockaddr_in *establishConnection(char *, int);
struct getSocket *acceptedIncomingClient(int);

struct sockaddr_in *establishConnection(char *ip, int port) {
	struct sockaddr_in* address = malloc(sizeof(struct sockaddr_in));
	address->sin_family = AF_INET;
	address->sin_port = htons(port);
	
	if(ip == "")
		address->sin_addr.s_addr = INADDR_ANY;
	else
		address->sin_addr.s_addr = inet_addr(ip);
	
	return address;
}


struct getSocket *acceptedIncomingClient(int serverSocket) {
	struct sockaddr_in clientAddr;
	int clientAddrSize = sizeof(struct sockaddr_in);
	int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
	
	struct getSocket *socket = malloc(sizeof(struct getSocket));
	socket->acceptedClientSocket = clientSocket;
	socket->addr = clientAddr;
	socket->accepted = (clientSocket == 0) ? true : false;
	if (socket->accepted) {
		socket->error = clientSocket;
	}
	
	return socket;
}

#endif
