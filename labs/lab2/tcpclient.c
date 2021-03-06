#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

void mkError(int condition, char *err){
  if(condition < 0){
    fprintf(stderr, "%s\n", err);
    exit(1);
  }
}

int main(int argc, char** argv){
  if(argc < 3){
    fprintf(stderr, "Must supply at least 2 parameters, the socket number and ip address\n");
  }
  int port = atoi(argv[1]);
  char* ip = argv[2];

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  mkError(sockfd, "There was an error creating the socket");

  struct sockaddr_in serveraddr; //server address can only be Ipv4
  serveraddr.sin_family = AF_INET; //Ipv4 again, will matter if cast to a more generic type 'socketaddr'
  //ports between 1024 and 49,000ish
  //actually 2 ports one to listen on and one the other program is listening on but the os takes care of this behind the scenes
  serveraddr.sin_port = htons(port); //lets os know there is a program listening on this port so that it will redirect packets here
  //127.0.0.1 is good testing address it is a loopback address, and always refers back to your own computer
  serveraddr.sin_addr.s_addr = inet_addr(ip); //use other computers ip address
  
  int  e = connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)); //sockaddr is more generic than sockaddr_in
  mkError(e, "There was an error with connecting\n");
  printf("Enter a message: ");
  char line[5000];
  fgets(line, 5000, stdin);
  send(sockfd, line, strlen(line) + 1,0); //third param is num bytes +1 to include null term char
  close(sockfd);
  return 0;
}
