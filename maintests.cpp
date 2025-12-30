// main_tcp_server.cpp
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "TCPServer.hpp"
#include "NetWrapper.hpp"
#include "TCPPacket.hpp"

int main(int ac, char **av)
{
    if (ac < 3) {
        std::cerr << "Usage: " << av[0] << " <NetPluginLibPath> <baseDir>\n";
        return 84;
    }

    const std::string pluginPath = av[1];
    const std::string baseDir = av[2];

    try {
        const auto net = std::make_shared<Net::NetWrapper>(pluginPath, baseDir);
        const auto proto = std::make_shared<Net::TCPPacket>(64 * 1024);

        Net::Server::TCPServer server(net, proto);
        server.configure("127.0.0.1", 4242);
        server.setNonBlocking(true);
        server.start();

        std::cout << "[OK] TCP server running on 127.0.0.1:4242\n";
        std::cout << "Press Ctrl+C to stop.\n";

        while (server.isRunning()) {
            server.readPackets();

            std::shared_ptr<Net::IPacket> pkt;
            while (server.popPacket(pkt)) {
                // Affiche le payload (si c'est du texte)
                const std::string msg(reinterpret_cast<const char *>(pkt->buffer()), pkt->size());
                std::cout << "[RX] " << pkt->size() << " bytes: " << msg << "\n";

                // Réponse ACK au même client (même address() -> mapping endpoint->socket côté TCPServer)
                const std::string ack = "ACK:" + msg;
                auto resp = proto->newPacket();
                resp->setAddress(*pkt->address());
                resp->setSize(ack.size());
                std::memcpy(resp->buffer(), ack.data(), ack.size());

                if (!server.sendPacket(*resp))
                    std::cout << "[WARN] sendPacket failed\n";
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        server.stop();
        return 0;

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 84;
    }
}
