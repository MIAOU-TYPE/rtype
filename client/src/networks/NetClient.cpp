/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetClient
*/

#include "NetClient.hpp"

namespace Network
{

    NetClient::NetClient() : ANetClient(), _netWrapper("NetPluginLib"), _ringBuffer(1024)
    {
        setRunning(false);
        if (_netWrapper.initNetwork() != 0)
            throw NetClientError("{NetClient::NetClient} Failed to initialize network");
    }

    NetClient::~NetClient()
    {
        close();
    }

    void NetClient::receivePackets()
    {
        if (!isRunning() || _socketFd == kInvalidSocket)
            return;
        auto pkt = std::make_shared<Net::UDPPacket>();
        socklen_t addrLen = sizeof(sockaddr_in);

        recvfrom_return_t received = _netWrapper.recvFrom(_socketFd, pkt->buffer(), Net::UDPPacket::MAX_SIZE, 0,
            reinterpret_cast<sockaddr *>(const_cast<sockaddr_in *>(pkt->address())), &addrLen);
        if (received <= 0)
            return;
        pkt->setSize(static_cast<size_t>(received));

        {
            std::scoped_lock lock(_packetDataMutex);
            if (!_ringBuffer.push(pkt))
                std::cerr << "{UDPServer::readPackets} Warning: RX buffer overflow, packet dropped\n";
        }
    }

    void NetClient::close()
    {
        setRunning(false);
        if (_socketFd != kInvalidSocket) {
            _netWrapper.closeSocket(_socketFd);
            _socketFd = kInvalidSocket;
            std::cout << "{UDPServer::stop} UDP Server stopped." << std::endl;
        }
        _netWrapper.cleanupNetwork();
    }

    void NetClient::start()
    {
        if (isRunning() || _socketFd != kInvalidSocket)
            throw NetClientError("{NetClient::start} Client is already running");

        try {
            setNonBlocking(true);
        } catch (const NetClientError &e) {
            if (_socketFd != kInvalidSocket)
                _netWrapper.closeSocket(_socketFd);
            _socketFd = kInvalidSocket;
            throw NetClientError(std::string("{NetClient::start}") + e.what());
        }
        setRunning(true);
        std::cout << "{NetClient::start} UDP Client started on " << _ip << ":" << _port << std::endl;
    }

    bool NetClient::sendPacket(const Net::IPacket &pkt)
    {
        return _netWrapper.sendTo(_socketFd, pkt.buffer(), pkt.size(), 0,
                   reinterpret_cast<const sockaddr *>(pkt.address()), sizeof(sockaddr_in))
            != -1;
    }

    bool NetClient::popPacket(std::shared_ptr<Net::IPacket> &pkt)
    {
        std::scoped_lock lock(_packetDataMutex);
        return _ringBuffer.pop(pkt);
    }

} // namespace Network