#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define PORT_NUMBER 9999
#define MAXLINE 4096

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAXLINE];
    char *hello = "Hello from server";

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed\n");
        exit(0);
    }

    bzero(&cliaddr, sizeof(cliaddr));
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT_NUMBER);

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
        perror("bind failed");
        exit(0);
    }

    int len, n;

    len = sizeof(cliaddr); //len is value/resuslt

    for (;;) {
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        if (strcmp(buffer, "bye") == 0) {
            break;
        }

        printf("Client : %s\n", buffer);
        sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        printf("Hello message sent.\n");

    }


    return 0;
}
