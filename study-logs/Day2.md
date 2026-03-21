---
title: Day 2
date: 2026-03-18
topic: TCP Client/Server Round-Trip in C
---

# Day 2 — TCP Client/Server Round‑Trip in C

Today I completed the full TCP round‑trip: a C server that accepts a client and a C client that connects to it. Both sides run an interactive loop, exchange data with `read()`/`write()`, and terminate cleanly when "exit" is sent. I also standardized the port to `8080` and practiced the full socket lifecycle on both ends.

---

## The Code

### TcpServer_IPv4.c (Server)

```c
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
  
void func(int connfd) 
{ 
    char buff[MAX]; 
    int n; 
    
    for (;;) { 
        bzero(buff, MAX); 
  
        read(connfd, buff, sizeof(buff)); 
        printf("From client: %s\t To client : ", buff); 
        
        bzero(buff, MAX); 
        n = 0; 
        
        while ((buff[n++] = getchar()) != '\n') 
            ; 
  
        write(connfd, buff, sizeof(buff)); 
  
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
} 
  
int main() 
{ 
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cli; 
  
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
  
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
        
    len = sizeof(cli); 
  
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server accept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server accept the client...\n"); 
  
    func(connfd); 
  
    close(sockfd); 
}
```

### TcpClient._IPv4.c (Client)

```c
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
        
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    func(sockfd);

    close(sockfd);
}
```

---

## Notes

1. This was the first day I completed the full request/response loop instead of stopping at `listen()`.
1. The server now goes one step further than Day 1: it calls `accept()` to create a connected socket for a specific client.
1. The client uses `connect()` to actively open the TCP session, then both programs exchange messages with `read()` and `write()`.
1. I standardized the examples in this project around port `8080` for the client/server round-trip files.
