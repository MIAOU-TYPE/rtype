/*
** EPITECH PROJECT, 2025
** POC RTYPE
** File description:
** udp_server_linux
*/

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        return 1;
    }

    std::cout << "UDP server running on port 5000..." << std::endl;

    while (true) {
        char buffer[1024];
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr *) &client_addr, &client_len);

        if (n < 0) {
            perror("recvfrom");
            continue;
        }

        std::string msg(buffer, n);
        std::cout << "[RECV] " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << " -> " << msg
                  << std::endl;

        if (msg == "exit") {
            std::cout << "Shutting down server." << std::endl;
            break;
        }
        std::string reply = "Server received: " + msg;
        sendto(sockfd, reply.c_str(), reply.size(), 0, (sockaddr *) &client_addr, client_len);
    }

    close(sockfd);
    return 0;
}
