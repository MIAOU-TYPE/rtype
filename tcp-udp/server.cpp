/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** udp-server
*/


#include <asio.hpp>
#include <iostream>
#include <thread>
#include <cassert>

using asio::ip::udp;
using asio::ip::tcp;

void udp_server(asio::io_context &ctx)
{
    udp::socket socket(ctx, udp::endpoint(udp::v4(), 5001));
    std::cout << "[UDP] Server listening on port 5001\n";

    while (true) {
        char buffer[1024];
        udp::endpoint sender;
        size_t received = socket.receive_from(asio::buffer(buffer), sender);

        std::string msg(buffer, received);
        std::cout << "[UDP] RECV from " << sender.address().to_string()
                  << ":" << sender.port() << " -> " << msg << "\n";

        std::string reply = "UDP OK: " + msg;
        socket.send_to(asio::buffer(reply), sender);
    }
}

void tcp_session(tcp::socket socket)
{
    try {
        std::cout << "[TCP] New client\n";

        for (;;) {
            char buffer[1024];
            asio::error_code ec;

            size_t n = socket.read_some(asio::buffer(buffer), ec);
            if (ec == asio::error::eof) {
                std::cout << "[TCP] Client disconnected\n";
                return;
            }

            std::string msg(buffer, n);
            std::cout << "[TCP] RECV -> " << msg << "\n";

            std::string reply = "TCP OK: " + msg;
            asio::write(socket, asio::buffer(reply));
        }
    } catch (std::exception &e) {
        std::cout << "[TCP] Error: " << e.what() << "\n";
    }
}

void tcp_server(asio::io_context &ctx)
{
    tcp::acceptor acceptor(ctx, tcp::endpoint(tcp::v4(), 5002));
    std::cout << "[TCP] Server listening on port 5002\n";

    while (true) {
        tcp::socket socket(ctx);
        acceptor.accept(socket);

        std::thread(tcp_session, std::move(socket)).detach();
    }
}

int main()
{
    asio::io_context ctx;

    std::thread udp_th(udp_server, std::ref(ctx));
    std::thread tcp_th(tcp_server, std::ref(ctx));

    udp_th.join();
    tcp_th.join();
}
