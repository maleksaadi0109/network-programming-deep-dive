#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8080
#define MAX_EVENTS 10

void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
    }
    
}

int setup_server(int port) {
    int master_socket;
    struct sockaddr_in address;
    int opt = 1;

    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(master_socket, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    set_nonblocking(master_socket);
    return master_socket;
}

int setup_epoll(int master_socket) {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1 failed");
        exit(EXIT_FAILURE);
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = master_socket;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, master_socket, &event) == -1) {
        perror("epoll_ctl: master_socket");
        exit(EXIT_FAILURE);
    }
    
    return epoll_fd;
}

void accept_new_connection(int epoll_fd, int master_socket) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int new_socket = accept(master_socket, (struct sockaddr *)&client_addr, &client_len);
    if (new_socket == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("accept failed");
        }
        return;
    }

    printf("New connection! Socket fd: %d, IP: %s\n", new_socket, inet_ntoa(client_addr.sin_addr));
    
    set_nonblocking(new_socket);

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = new_socket;
    
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
        perror("epoll_ctl: new_socket");
        close(new_socket);
    }
}

void handle_client_data(int client_socket) {
    char buffer[1024] = {0};
    int valread = read(client_socket, buffer, sizeof(buffer) - 1);
    
    if (valread == 0) {
        printf("Client disconnected! Socket fd: %d\n", client_socket);
        close(client_socket); 
    } 
    else if (valread > 0) {
        buffer[valread] = '\0';
        printf("Received from client (fd %d): %s", client_socket, buffer);
        send(client_socket, buffer, strlen(buffer), 0);
    } 
    else {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("read error");
            close(client_socket);
        }
    }
}

int main() {
    int master_socket = setup_server(PORT);
    int epoll_fd = setup_epoll(master_socket);
    
    struct epoll_event events[MAX_EVENTS];

    printf("epoll Server is running on port %d...\n", PORT);

    for (;;) {
        
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait failed");
            exit(EXIT_FAILURE);
                }
          
        
        for (int n = 0; n < nfds; n++) {
            if (events[n].data.fd == master_socket) {
               accept_new_connection(epoll_fd, master_socket);
            } else {
                handle_client_data(events[n].data.fd);
            }
        }
    }

    close(master_socket);
    close(epoll_fd);
    return 0;
}
