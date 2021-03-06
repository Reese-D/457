#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

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
    fprintf(stderr, "message from client: %s\n", line);
    int fd = open(line, O_RDONLY);
    struct stat f_stat;
    fstat(fd, &f_stat);
    int size = f_stat.st_size;
    off_t offset = 0;
    char filesize[256];
    sprintf(filesize, "%d", size);
    n = send(clientsocket, filesize, 256, 0);
    fprintf(stderr, "Sent %d bytes\n", size);
    while(((sendfile(clientsocket, fd, &offset, 5000)) > 0) && (size > 0)){
      printf("looped\n");
      size -= 5000;
    }
    
    //    if(n < 0){
      //      fprintf(stderr, "couldn't write message to client\n");
      //    }
    close(clientsocket);
  }

}

