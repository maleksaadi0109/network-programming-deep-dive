#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 

#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 
  

  
int main() 
{ 
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cli; 
    time_t ticks;
    char buff[MAX];
  
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
        
    bzero(&servaddr, sizeof(servaddr)); 
  
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
     Listen(sockfd, LISTENQ);
        
    len = sizeof(cli); 
  
    
    if (connfd < 0) { 
        printf("server accept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server accept the client...\n"); 
  
    
     for(;;){
       connfd = accept(sockfd, (SA*)&cli, &len); 
       ticks=time(NULL);
       snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
       Write(connfd, buff, strlen(buff));
       close(connfd);
     }
     return 0;
    
}