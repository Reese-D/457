#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv){
  if(argc < 2){
    fprintf(stderr, "Must supply port number as parameter\n");
    return 1;
  }
  int port = atoi(argv[1]);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in serveraddr, clientaddr;
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(port);
  serveraddr.sin_addr.s_addr = INADDR_ANY;

  int e = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
  if(e < 0){
    fprintf(stderr, "there was an error binding to the address\n");
    return 1;
  }
  listen(sockfd, 10);

  while(1){
    socklen_t len = sizeof(clientaddr);
    int clientsocket = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
    char line[5000];
    int n = recv(clientsocket, line, 5000, 0);
    if(n < 0){
      fprintf(stderr, "couldn't receive message from client\n");
    }
    fprintf(stderr, "message from client: %s", line);
    send(sockfd, line, strlen(line),0);
    close(clientsocket);
  }

}

