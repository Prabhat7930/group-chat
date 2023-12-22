#include "SocketUtils.h"

void *receiveMsg(void *);
void startListeningToServer(int);
void sendMessage(char *, ssize_t, int, char *);

int main(int argc, char **argv) {
	int serverPort, clientSocket;
	char *ip, *line, *username;
	size_t lineSize;
	struct sockaddr_in* serverAddr; 
	
	serverPort = atoi(argv[1]);
	username = argv[2];
	
	//SOCKET CREATION
	if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("error creating socket");
		exit(EXIT_FAILURE);
	}
	printf("socket created successfully\n");
	
	//CONNECTING TO SERVER
	ip = "127.0.0.1";
	serverAddr = establishConnection(ip, serverPort);
	if(connect(clientSocket, (struct sockaddr*)serverAddr, sizeof(*serverAddr)) < 0) {
		perror("error during connection establishment");
		exit(EXIT_FAILURE);
	}
	printf("connection established\n");
	
	//SEND MESSAGE TO SERVER
	line = NULL;
	lineSize = 0;
	printf("type what you want to send to server\n");
	
	startListeningToServer(clientSocket);
	sendMessage(line, lineSize, clientSocket, username);
	
	
	close(clientSocket);
	
	return 0;
}

void startListeningToServer(int clientSocket) {	
	int *clientSocketPtr = malloc(sizeof(int));
	*clientSocketPtr = clientSocket;
	pthread_t id;
	
	pthread_create(&id, NULL, receiveMsg, (void *)clientSocketPtr);
	
}

void *receiveMsg(void *arg) {
	int *clientSocketPtr = (int *)arg;
	int clientSocket = *clientSocketPtr;
	char buffer[1024];
	
	while(true) {
		ssize_t amountReceived = recv(clientSocket, buffer, 1024, 0);
		if (amountReceived > 0) {
			buffer[amountReceived] = 0;
			printf("%s\n", buffer);
		}
		else
			break;
	}	
}


void sendMessage(char *line, ssize_t lineSize, int clientSocket, char *username) {
	while(true) {
		ssize_t charCount = getline(&line, &lineSize, stdin);	
		if (charCount > 0) {
			if (strcmp(line, "exit\n") == 0)
				break;
			char msg[512];
			strcpy(msg, username);
			strcat(msg, ": ");
			strcat(msg, line);
			ssize_t msgCharCount = strlen(msg);
			size_t amountSend = send(clientSocket, msg, msgCharCount, 0);
			printf("\n");		
		}
		
	}
	free(line);
}
 
