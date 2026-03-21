#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <time.h>
#include <unistd.h> 

#define MAX 80 
#define PORT 8080 
#define LISTENQ 5
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
  
    if (listen(sockfd, LISTENQ) != 0) {
        printf("Listen failed...\n");
        close(sockfd);
        exit(1);
    }
    else {
        printf("Server listening..\n");
    }
        
    len = sizeof(cli); 

    for (;;) {
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            printf("server accept failed...\n");
            close(sockfd);
            exit(1);
        }

        printf("server accepted a client...\n");

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));
        close(connfd);
    }

    close(sockfd);
    return 0;
    
}
