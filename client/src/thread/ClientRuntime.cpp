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
        _renderer = std::make_shared<Graphics::SFMLRenderer>();
        _display = std::make_shared<Display::DisplayInit>(_renderer);
        _event = std::make_shared<Events::EventInit>(_renderer);
    }

    ClientRuntime::~ClientRuntime()
    {
        stop();
        _client = nullptr;
    }

    void ClientRuntime::wait()
    {
        std::unique_lock lock(_mutex);
        _cv.wait(lock, [this]() {
            return _stopRequested.load();
        });
    }

    void ClientRuntime::start()
    {
        try {
            _client->start();
            _client->sendPacket(*_packetFactory->makeBase(Net::Protocol::CONNECT));
        } catch (const std::exception &e) {
            throw ClientRuntimeError(std::string("{ClientRuntime::start} Failed to start client: ") + e.what());
        }

        _running = true;
        _receiverThread = std::thread(&ClientRuntime::runReceiver, this);
        _updateThread = std::thread(&ClientRuntime::runUpdater, this);
        runDisplay();
    }

    void ClientRuntime::stop()
    {
        if (_stopRequested.exchange(true))
            return;

        _running = false;
        _cv.notify_all();

        if (_receiverThread.joinable())
            _receiverThread.join();
        if (_updateThread.joinable())
            _updateThread.join();
        if (_client) {
            _client->sendPacket(*_packetFactory->makeBase(Net::Protocol::DISCONNECT));
            _client->close();
        }
    }

    void ClientRuntime::runReceiver() const
    {
        while (_running) {
            _client->receivePackets();
        }
    }

    void ClientRuntime::runUpdater() const
    {
        while (_running) {
            if (std::shared_ptr<Net::IPacket> pkt = nullptr; _client->popPacket(pkt)) {
                _packetRouter->handlePacket(pkt);
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    }

    void ClientRuntime::runDisplay()
    {
        _display->init(800, 600);

        while (_running) {
            _display->run();
            _event->run();
            if (!_display->isWindowOpen() || !_event->isWindowOpen()) {
                stop();
                break;
            }
        }
    }
} // namespace Thread