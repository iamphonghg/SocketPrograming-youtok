#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUFF_SIZE 4096
#define PORT 1555

void receive_image(int, char *);

int main(int argc, char *argv[])
{
  int sockfd, connfd;
  struct sockaddr_in servaddr, cliaddr;
  int sin_size;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket creation failed\n");
    exit(0);
  }

  bzero(&cliaddr, sizeof(cliaddr));
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
  {
    perror("bind failed");
    exit(0);
  }

  if ((listen(sockfd, 5)) != 0)
  {
    printf("Listen failed...\n");
    exit(0);
  }
  else
    printf("Server listening...\n");

  sin_size = sizeof(struct sockaddr_in);
  if ((connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &sin_size)) == -1)
  {
    perror("\nError: ");
    close(sockfd);
    exit(1);
  }

  receive_image(connfd, "newFile.mp4");

  close(connfd);
  return 0;
}

void receive_image(int sockfd, char *imageFileName)
{
  FILE *newFile = fopen(imageFileName, "wb");

  ssize_t n;
  char buff[BUFF_SIZE] = {0};
  while ((n = recv(sockfd, buff, BUFF_SIZE, 0)) > 0)
  {
    if (n == -1)
    {
      printf("Receive image failed!\n");
      exit(1);
    }

    if (fwrite(buff, sizeof(char), n, newFile) != n)
    {
      printf("Write file failed!\n");
      exit(1);
    }
    bzero(&buff, sizeof(buff));
  }
  fclose(newFile);
}
