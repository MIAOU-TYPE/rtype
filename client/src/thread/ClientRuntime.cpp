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
            throw ClientRuntimeError("NetClient is null");
        _packetFactory = std::make_shared<Network::ClientPacketFactory>(std::make_shared<Net::UDPPacket>());
        _resourceManager = std::make_shared<Resources::EmbeddedResourceManager>();
        _renderer = std::make_shared<Graphics::SFMLRenderer>(_resourceManager);
        _display = std::make_shared<Display::DisplayInit>(_renderer);
        _event = std::make_shared<Events::EventInit>(_renderer, _client, _packetFactory);
        _inputEventManager = std::make_shared<Events::InputEventManager>();
        _inputHandler = std::make_unique<Input::SFMLInputHandler>(_inputEventManager);
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
        _display->init(800, 600);
        _gameScene = _display->getGameScene();
        if (!_gameScene)
            throw ClientRuntimeError("GameScene not created by DisplayInit");
        _packetRouter = std::make_shared<Ecs::PacketRouter>(_gameScene->getGameWorldPtr());
        _client->start();
        _client->sendPacket(*_packetFactory->makeBase(Net::Protocol::CONNECT));
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

    void ClientRuntime::runReceiver()
    {
        while (_running) {
            _client->receivePackets();
        }
    }

    void ClientRuntime::runUpdater()
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