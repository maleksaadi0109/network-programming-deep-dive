#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8080

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

void handle_new_connection(int master_socket, fd_set *master_set, int *max_sd) {
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("Accept failed");
        }
        return;
    }

    printf("New connection! Socket fd: %d, IP: %s, Port: %d\n",
           new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
           
    set_nonblocking(new_socket);
    FD_SET(new_socket, master_set);

    if (new_socket > *max_sd) {
        *max_sd = new_socket;
    }
}

void handle_client_data(int client_socket, fd_set *master_set) {
    char buffer[1024] = {0};
    int valread = read(client_socket, buffer, sizeof(buffer) - 1);
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if (valread == 0) {
        getpeername(client_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        printf("Client disconnected! IP: %s, Port: %d\n", 
               inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        close(client_socket);
        FD_CLR(client_socket, master_set);
    } 
    else if (valread > 0) {
        buffer[valread] = '\0';
        send(client_socket, buffer, strlen(buffer), 0);
        printf("%s\n", buffer);
    } 
    else {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("Read error or connection reset");
            close(client_socket);
            FD_CLR(client_socket, master_set);
        }
    }
}

int main() {
    fd_set master_set, readfds;
    int master_socket = setup_server(PORT);
    
    printf("Server is running on port %d...\n", PORT);

    FD_ZERO(&master_set);
    FD_SET(master_socket, &master_set);
    int max_sd = master_socket;

    while (1) {
        readfds = master_set;

        if (select(max_sd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }
        

        for (int i = 0; i <= max_sd; i++) {
            if (FD_ISSET(i, &readfds)) {
                if (i == master_socket) {
                    handle_new_connection(master_socket, &master_set, &max_sd);
                } 
                else {
                    handle_client_data(i, &master_set);
                }
            }
        }
    }
    return 0;
}
