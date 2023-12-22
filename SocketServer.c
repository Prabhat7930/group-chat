#include "SocketUtils.h"

/*struct argsToThread {
	int acceptedClientSocket;
	pthread_t id;
};*/


void startAcceptingClient(int);
void printDataOnSeparateThread(struct getSocket *);
void *receiveIncomingMsg(void *);
void sendToOtherClients(char [], int);

struct getSocket allClients[10];
int clientCount = 0;

int main(int argc, char** argv) {
	int serverPort, serverSocket;
	struct sockaddr_in *serverAddr;
	struct getSocket *clientSocket;
	
	serverPort = atoi(argv[1]);
	
	//SOCKET CREATION
	if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("error creating socket"); 
		exit(EXIT_FAILURE);
	}
	printf("server socket created!\n");
	
	//BINDING IP TO PORT
	serverAddr = establishConnection("", serverPort);
	if(bind(serverSocket, (struct sockaddr*)serverAddr, sizeof(*serverAddr)) < 0) {
		perror("error binding port to ip"); 
		exit(EXIT_FAILURE);
	}
	printf("binding success!\n");
	
	//LISTENING TO CLIENTS
	if((listen(serverSocket, 10)) < 0) {
		perror("error in server listening\n"); 
		exit(EXIT_FAILURE);	
	}
	printf("listening success!\n");		
	
	startAcceptingClient(serverSocket);
	
	shutdown(serverSocket, SHUT_RDWR);

	return 0;
}


void startAcceptingClient(int serverSocket) {
	while(true) {
		struct getSocket *clientSocket = acceptedIncomingClient(serverSocket);
		allClients[clientCount++] = *clientSocket;
		if (clientSocket->acceptedClientSocket < 0) {
			perror("connection not accepted!");
			exit(EXIT_FAILURE);
		}
		printDataOnSeparateThread(clientSocket); 
		
	}
}

void printDataOnSeparateThread(struct getSocket *cSocket) {
	pthread_t id;
	printf("Client connected successfully with clientID %ld\n",id);
	
	pthread_create(&id, NULL, receiveIncomingMsg, (void *)&(cSocket->acceptedClientSocket));
}

void *receiveIncomingMsg(void *arg) {
	int acceptedClientSocket = *((int *)arg);
	char buffer[1024];
	while(true) {		
		ssize_t amountReceived = recv(acceptedClientSocket, buffer, 1024, 0);
		if (amountReceived > 0) {
			buffer[amountReceived] = 0;
			printf("Response was %s\n", buffer);
			
			sendToOtherClients(buffer, acceptedClientSocket);
		}
		else
			break;
	}

	close(acceptedClientSocket);
	pthread_cancel(pthread_self());
}


void sendToOtherClients(char buffer[], int acceptedClientSocket) {
	for(int i = 0; i < clientCount; i++) {
		if (allClients[i].acceptedClientSocket != acceptedClientSocket) {
			send(allClients[i].acceptedClientSocket, buffer, strlen(buffer), 0);
		}
	}
}
