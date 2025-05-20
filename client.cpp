#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUF_SIZE 1024

void scenario_normal_data() {
    std::cout << "\n[Scenario 1] NORMAL_DATA:Hello" << std::endl;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return;
    }

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return;
    }

    const char* msg = "NORMAL_DATA:Hello";
    send(sock, msg, strlen(msg), 0);
    std::cout << "Sent: " << msg << std::endl;

    char buffer[BUF_SIZE] = {0};
    int received = recv(sock, buffer, BUF_SIZE - 1, 0);
    if (received > 0) {
        buffer[received] = '\0'; 
        std::cout << "Received: " << buffer << std::endl;
    } else {
        std::cerr << "Error receiving data" << std::endl;
    }

    close(sock);
}

void scenario_urgent_data() {
    std::cout << "\n[Scenario 2] SEND_URGENT_REQUEST + urgent byte" << std::endl;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return;
    }

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return;
    }

    const char* cmd = "SEND_URGENT_REQUEST";
    send(sock, cmd, strlen(cmd), 0);
    std::cout << "Sent: " << cmd << std::endl;

    char urgent_byte = 'U';
    send(sock, &urgent_byte, 1, MSG_OOB); 
    std::cout << "Sent urgent byte: " << urgent_byte << std::endl;

    char buffer[BUF_SIZE] = {0};
    int received = recv(sock, buffer, BUF_SIZE - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    } else {
        std::cerr << "Error receiving data" << std::endl;
    }

    close(sock);
}

void scenario_no_urgent_data() {
    std::cout << "\n[Scenario 3] SEND_URGENT_REQUEST but NO urgent byte" << std::endl;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return;
    }

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return;
    }

    const char* cmd = "SEND_URGENT_REQUEST";
    send(sock, cmd, strlen(cmd), 0);
    std::cout << "Sent: " << cmd << std::endl;

    shutdown(sock, SHUT_WR); 

    char buffer[BUF_SIZE] = {0};
    int received = recv(sock, buffer, BUF_SIZE - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    } else {
        std::cerr << "Error receiving data" << std::endl;
    }

    close(sock);
}

void scenario_unknown_command() {
    std::cout << "\n[Scenario 4] Unknown command" << std::endl;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return;
    }

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return;
    }

    const char* msg = "TEST_GARBAGE";
    send(sock, msg, strlen(msg), 0);
    std::cout << "Sent: " << msg << std::endl;

    char buffer[BUF_SIZE] = {0};
    int received = recv(sock, buffer, BUF_SIZE - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    } else {
        std::cerr << "Error receiving data" << std::endl;
    }

    close(sock);
}

int main() {
    scenario_normal_data();
    scenario_urgent_data();
    scenario_no_urgent_data();
    scenario_unknown_command();
    return 0;
}
