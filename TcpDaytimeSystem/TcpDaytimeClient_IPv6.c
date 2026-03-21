#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX 80
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[MAX];
    int n;
    
    
    while ( (n = read(sockfd, buff, sizeof(buff) - 1)) > 0 ) {
        
        buff[n] = 0; 
        
       
        if (fputs(buff, stdout) == EOF) {
            printf("fputs error\n");
            exit(1);
        }
    }

    if (n < 0) {
        printf("read error\n");
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in6 servaddr;

    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(1);
    }
    else {
        printf("Socket successfully created..\n");
    }
        
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, "2610:20:6f15:15::27", &servaddr.sin6_addr);
    servaddr.sin6_port = htons(13); 

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(1);
    }
    else {
        printf("connected to the server..\n");
    }

    func(sockfd);

    close(sockfd);
    
    return 0;
}
