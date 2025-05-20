#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BACKLOG 5
#define BUF_SIZE 1024

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr{}, client_addr{};
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUF_SIZE];
    char oob_byte;

    
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        return 1;
    }

    
    int opt = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_sock);
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_sock);
        return 1;
    }

  
    if (listen(server_sock, BACKLOG) < 0) {
        perror("listen");
        close(server_sock);
        return 1;
    }

    std::cout << "Server listening on 127.0.0.1:" << PORT << std::endl;

    while (true) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("accept");
            continue; 
        }

        memset(buffer, 0, BUF_SIZE);
        
        int bytes_received = recv(client_sock, buffer, BUF_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("recv");
            close(client_sock);
            continue; 
        }

        buffer[bytes_received] = '\0';

        if (strcmp(buffer, "NORMAL_DATA:Hello") == 0) {
            send(client_sock, "SERVER_ACK:Hello", 16, 0);
        }
      
        else if (strcmp(buffer, "SEND_URGENT_REQUEST") == 0) {
            int oob_result = recv(client_sock, &oob_byte, 1, MSG_OOB);
            if (oob_result == 1) {
                std::string response = "SERVER_URGENT_ACK:";
                response += oob_byte; 
                send(client_sock, response.c_str(), response.length(), 0);
            } else {
                send(client_sock, "SERVER_NO_URGENT_DATA", 22, 0);
            }
        }
     
        else {
            send(client_sock, "SERVER_UNKNOWN_COMMAND", 23, 0);
        }

        
        close(client_sock);
    }

    
    close(server_sock);
    return 0;
}
