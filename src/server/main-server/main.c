#include "prototype.h"

int main(int argc, char *argv[])
{
  int sockfd, connfd;
  struct sockaddr_in servaddr, cliaddr;

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

  int len, n;

  len = sizeof(cliaddr);

  int no_threads = 0;
  pthread_t threads[50];

  for (;;)
  {
    if ((listen(sockfd, 50)) != 0)
    {
      printf("Listen failed...\n");
      exit(0);
    }
    else
      printf("Server listening..\n");
    len = sizeof(cliaddr);

    connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
    if (pthread_create(&threads[no_threads], NULL, handle_request, &connfd) < 0)
    {
      perror("Could not create thread");
      return 1;
    }
    if (connfd < 0)
    {
      printf("server accept failed...\n");
      exit(0);
    }
    else
      printf("server accept the client...\n");

    puts("Handler assigned");
    no_threads++;
  }

  int k = 0;
  for (k = 0; k < 50; k++)
  {
    pthread_join(threads[k], NULL);
  }
  close(sockfd);
  return 0;
}

