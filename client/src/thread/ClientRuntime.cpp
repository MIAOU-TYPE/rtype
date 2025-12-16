/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ClientRuntime
*/

#include "ClientRuntime.hpp"

namespace Thread
{
    ClientRuntime::ClientRuntime(
        const std::shared_ptr<Network::INetClient> &client,
        const std::shared_ptr<Game::GameScene> &scene)
        : _client(client), _gameScene(scene)
    {
        if (!_client)
            throw ClientRuntimeError("NetClient is null");
        if (!_gameScene)
            throw ClientRuntimeError("GameScene is null");
        _packetFactory = std::make_shared<Network::ClientPacketFactory>(
            std::make_shared<Net::UDPPacket>());
        _packetRouter = std::make_shared<Ecs::PacketRouter>(
            _gameScene->getGameWorldPtr());
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
            _client->sendPacket(*_packetFactory->makeBase(Net::Protocol::CONNECT));
        } catch (std::exception &e) {
            throw ClientRuntimeError(std::string("{ClientRuntime::start} Failed to start client: ") + e.what());
        }
        _running = true;
        _receiverThread = std::thread(&ClientRuntime::runReceiver, this);
        _updateThread = std::thread(&ClientRuntime::runUpdater, this);
    }

    void ClientRuntime::stop()
    {
        if (_client) {
            _client->sendPacket(*_packetFactory->makeBase(Net::Protocol::DISCONNECT));
            _client->close();
        }
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
                _packetRouter->handlePacket(pkt);
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    }
} // namespace Thread