/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ClientRuntime
*/

#include "ClientRuntime.hpp"

namespace Thread
{

    ClientRuntime::ClientRuntime(const std::shared_ptr<Network::INetClient> &client) : _client(client)
    {
        if (!_client)
            throw std::invalid_argument("{ClientRuntime::ClientRuntime} client pointer is null");
        _packetFactory = std::make_shared<Network::ClientPacketFactory>(std::make_shared<Net::UDPPacket>());
        _entitiesFactory = std::make_shared<Ecs::EntitiesFactory>();
    }

    ClientRuntime::~ClientRuntime()
    {
        if (!_stopRequested)
            stop();
        _client = nullptr;
    }

    void ClientRuntime::wait()
    {
        std::unique_lock lock(_mutex);
        _cv.wait(lock, [this]() {
            return _stopRequested;
        });
    }

    void ClientRuntime::start()
    {
        try {
            _client->start();
        } catch (std::exception &e) {
            throw std::runtime_error(std::string("{ClientRuntime::start} Failed to start client: ") + e.what());
        }
        _running = true;
        _receiverThread = std::thread(&ClientRuntime::runReceiver, this);
        _updateThread = std::thread(&ClientRuntime::runUpdater, this);
    }

    void ClientRuntime::stop()
    {
        {
            std::scoped_lock lock(_mutex);
            _stopRequested = true;
            _running = false;
        }
        _cv.notify_all();

        if (_receiverThread.joinable())
            _receiverThread.join();
        if (_updateThread.joinable())
            _updateThread.join();
    }

    void ClientRuntime::runReceiver() const
    {
        while (_client->isRunning()) {
            _client->receivePackets();
        }
    }

    void ClientRuntime::runUpdater() const
    {
        while (_client->isRunning()) {
            if (std::shared_ptr<Net::IPacket> pkt = nullptr; _client->popPacket(pkt)) {
                _entitiesFactory->handlePacket(pkt);
            }
        }
    }

} // namespace Thread