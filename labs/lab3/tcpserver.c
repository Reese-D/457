#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
/* struct clientinfo{
  struct sockaddr_in addr;
  int socket;
  }*/

void* handleclient(void *arg){
  /* struct clientinfo ci = *(struct clientinfo*)arg; */
  /* int clientsocket = ci.socket; */
  /* struct socketaddr_in clientaddr = ci.addr; */
  int clientsocket = *(int*)arg;
  char line[5000];
  ssize_t n = recv(clientsocket, line, 5000, 0);
  if(n == -1){
    printf("recv error\n");
    return arg;
  }
  printf("got from the client: %s\n", line);
  close(clientsocket);
  return arg;
}

int main(int argc, char** argv){
  int sockfd = socket(AF_INET,SOCK_STREAM, 0);

  struct sockaddr_in serveraddr, clientaddr;
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(4556);
  serveraddr.sin_addr.s_addr = INADDR_ANY;
  
  int e = bind(sockfd, (struct sockaddr*)&serveraddr,sizeof(serveraddr));
  if(e<0){
    //handle
  }

  listen(sockfd,10);

  while(1){
    socklen_t len  = sizeof(clientaddr);
    int clientsocket = accept(sockfd,(struct sockaddr*)&clientaddr,&len);
    pthread_t child;
    pthread_create(&child,NULL,handleclient,&clientsocket);
    pthread_detach(child); //pthread join waits for return value, detach doesn't
  }
  return 0;
}
