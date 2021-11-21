/*UDP Echo Client*/
#include <stdio.h>    
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 100

int main(int argc, char* argv[]){
	if (argc != 3) {
		printf("Please input IP address and port number!\n");
		exit(0);
	}

	int client_sock;
	char* ipAddress = argv[1];
	int portNumber = atoi(argv[2]); 
	char buff[BUFF_SIZE], numberReceive[BUFF_SIZE], stringReceive[BUFF_SIZE];
	struct sockaddr_in server_addr;
	int bytes_sent, bytes_received, sin_size;
	
	if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {  
		perror("\nError: ");
		exit(0);
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNumber);
	server_addr.sin_addr.s_addr = inet_addr(ipAddress);
	

	while (1) {
		printf("Insert string to send: ");
		if (! scanf("%[^\n]", buff)) break; 
		getchar();
		sin_size = sizeof(struct sockaddr);

		bytes_sent = sendto(client_sock, buff, strlen(buff), 0, (struct sockaddr *) &server_addr, sin_size);
		if (bytes_sent < 0)
		{
			perror("Error: ");
			close(client_sock);
			return 0;
		}
		bytes_received = recvfrom(client_sock, numberReceive, BUFF_SIZE-1, 0, (struct sockaddr *) &server_addr, &sin_size);
		if (bytes_received < 0)
		{
			perror("Error: ");
			close(client_sock);
			return 0;
		}
		numberReceive[bytes_received] = '\0';
		bytes_received = recvfrom(client_sock, stringReceive, BUFF_SIZE-1, 0, (struct sockaddr *) &server_addr, &sin_size);
		if (bytes_received < 0)
		{
			perror("Error: ");
			close(client_sock);
			return 0;
		}
		stringReceive[bytes_received] = '\0';
		printf("Reply from server: \n");
		if (strcmp(numberReceive, "")) {
			printf("%s\n", numberReceive);
		}
		if (strcmp(stringReceive, "")) {
			printf("%s\n", stringReceive);
		}
	}
	close(client_sock);
	return 0;
}
