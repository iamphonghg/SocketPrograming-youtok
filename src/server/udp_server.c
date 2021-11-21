#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUFF_SIZE 100

int splitBuffer(char*, char*, char*);

int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("Please input just 1 argument as port number!\n");
		exit(0);
	}
	
	int server_sock;
	int portNumber = atoi(argv[1]); 
	char buff1[BUFF_SIZE], buff2[BUFF_SIZE], stringStr[BUFF_SIZE], numberStr[BUFF_SIZE];
	int bytes_sent1, bytes_sent2, bytes_received1, bytes_received2;
	struct sockaddr_in server; 
	struct sockaddr_in client1, client2; 
	int sin_size1, sin_size2;

	if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {  
		perror("\nError: ");
		printf("check point\n");
		exit(0);
	}
	
	server.sin_family = AF_INET;         
	server.sin_port = htons(portNumber);  
	server.sin_addr.s_addr = INADDR_ANY;  
	bzero(&(server.sin_zero), 8); 
  
	if (bind(server_sock, (struct sockaddr*) &server, sizeof(struct sockaddr)) == -1) { 
		perror("\nError: ");
		printf("check point\n");
		exit(0);
	}     
	
	while (1) {
    	sin_size1 = sizeof(struct sockaddr_in);
		sin_size2 = sizeof(struct sockaddr_in);

		bytes_received1 = recvfrom(server_sock, buff1, BUFF_SIZE-1, 0, (struct sockaddr *) &client1, &sin_size1);
		bytes_received2 = recvfrom(server_sock, buff2, BUFF_SIZE-1, 0, (struct sockaddr *) &client2, &sin_size2);
		
		if (bytes_received1 < 0) {
			perror("\nError: ");
			printf("check point\n");
		} else {
			buff1[bytes_received1] = '\0';
			if (! splitBuffer(buff1, stringStr, numberStr)) {
				strcpy(numberStr, "Error");
				strcpy(stringStr, "");
			}
		}

		bytes_sent1 = sendto(server_sock, numberStr, sizeof(numberStr), 0, (struct sockaddr *) &client2, sin_size2); 
		if (bytes_sent1 < 0) {
			perror("\nError: ");
			printf("check point\n");
		}

		bytes_sent1 = sendto(server_sock, stringStr, sizeof(stringStr), 0, (struct sockaddr *) &client2, sin_size2); 
		if (bytes_sent1 < 0) {
			perror("\nError: ");					
			printf("check point\n");
		}

		if (bytes_received2 < 0) {
			perror("\nError: ");
			printf("check point\n");
		} else {
			buff2[bytes_received2] = '\0'; 
			if (! splitBuffer(buff2, stringStr, numberStr)) {
				strcpy(numberStr, "Error");
				strcpy(stringStr, "");
			}
		}
		
		bytes_sent2 = sendto(server_sock, numberStr, sizeof(numberStr), 0, (struct sockaddr *) &client1, sin_size1); 
		if (bytes_sent2 < 0) {
			perror("\nError: ");					
			printf("check point\n");
		}
		bytes_sent2 = sendto(server_sock, stringStr, sizeof(stringStr), 0, (struct sockaddr *) &client1, sin_size1); 
		if (bytes_sent2 < 0) {
			perror("\nError: ");					
			printf("check point\n");
		}
	}
	
	close(server_sock);
	
	return 0;
}

int splitBuffer(char* buffer, char* stringStr, char* numberStr) {
	int x = 0, y = 0, z = 0;
	while(buffer[x]) {
		if (isdigit(buffer[x])) {
			numberStr[y++] = buffer[x];
		} else if (isalpha(buffer[x])) {
			stringStr[z++] = buffer[x];
		} else {
			return 0;
		}
		x++;
	}
	numberStr[y] = '\0';
	stringStr[z] = '\0';
	return 1;
}
