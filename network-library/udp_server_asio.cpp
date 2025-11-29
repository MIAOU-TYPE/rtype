/*
** EPITECH PROJECT, 2025
** POC RTYPE
** File description:
** udp_server_asio.cpp
*/

#include <asio.hpp>
#include <cassert>
#include <iostream>

static inline std::string recv(asio::ip::udp::socket &socket, asio::ip::udp::endpoint &sender_endpoint, char *buffer,
    size_t buffer_size, size_t &bytes_received)
{
    bytes_received = socket.receive_from(asio::buffer(buffer, buffer_size), sender_endpoint);

    std::string msg(buffer, bytes_received);

    std::cout << "[RECV] " << sender_endpoint.address().to_string() << ":" << sender_endpoint.port() << " -> " << msg
              << std::endl;

    return msg;
}

static inline void send_msg(
    asio::ip::udp::socket &socket, const asio::ip::udp::endpoint &endpoint, const std::string &message)
{
    size_t bytes_sent = socket.send_to(asio::buffer(message), endpoint);
    assert(bytes_sent == message.size());

    std::cout << "[SEND] " << endpoint.address().to_string() << ":" << endpoint.port() << " <- " << message
              << std::endl;
}

int main()
{
    try {
        asio::io_context io_context;

        asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 5000));

        std::cout << "UDP server running on port 5000..." << std::endl;

        while (true) {
            char data[1024];
            asio::ip::udp::endpoint sender_endpoint;
            size_t bytes_received = 0;

            std::string msg = recv(socket, sender_endpoint, data, sizeof(data), bytes_received);

            std::string reply = "Server received: " + msg;

            send_msg(socket, sender_endpoint, reply);
        }

    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
