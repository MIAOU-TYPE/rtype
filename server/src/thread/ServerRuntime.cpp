/*
** EPITECH PROJECT, 2025
** RType
** File description:
** ServerRuntime.cpp
*/

#include "ServerRuntime.hpp"

using namespace Net::Thread;

ServerRuntime::ServerRuntime(const std::shared_ptr<Server::IServer> &server) : _server(server)
{
    _sessionManager = std::make_shared<SessionManager>();
    _gameServer = std::make_shared<Game::GameServer>(_sessionManager, _server);
    _packetRouter = std::make_shared<PacketRouter>(_sessionManager, _gameServer);
}

ServerRuntime::~ServerRuntime()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_stopRequested)
        stop();
    _server = nullptr;
}

void ServerRuntime::wait()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait(lock, [this]() {
        return _stopRequested;
    });
}

void ServerRuntime::start()
{
    _server->start();
    _receiverThread = std::thread(&ServerRuntime::runReceiver, this);
    _processorThread = std::thread(&ServerRuntime::runProcessor, this);
}

void ServerRuntime::stop()
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _stopRequested = true;
    }
    _cv.notify_all();

    _server->setRunning(false);
    if (_receiverThread.joinable())
        _receiverThread.join();
    if (_processorThread.joinable())
        _processorThread.join();
    _server->stop();
}

void ServerRuntime::runReceiver() const
{
    while (_server->isRunning()) {
        _server->readPackets();
    }
}

void ServerRuntime::runProcessor() const
{
    auto last = std::chrono::steady_clock::now();
    while (_server->isRunning()) {
<<<<<<< HEAD
        std::shared_ptr<Net::IPacket> packet = nullptr;
        if (_server->popPacket(packet)) {
            _packetRouter->handlePacket(packet);
            // TODO:
            //  When the real GameServer is implemented (instead of tempMessageSink),
            //  call gameServer->update(dt) here.
            //  This will:
            //  - process inputs stored in ECS components
            //  - tick movement / physics / collision / AI systems
            //  - compute world state changes
            //  - generate a snapshot for clients using PacketFactory
            //
            //  This is the place where the game simulation runs every frame.
=======
        if (std::shared_ptr<Net::IServerPacket> pkt = nullptr; _server->popPacket(pkt)) {
            _packetRouter->handlePacket(pkt);

            auto now = std::chrono::steady_clock::now();
            const float dt = std::chrono::duration<float>(now - last).count();
            last = now;
            _gameServer->update(dt);
>>>>>>> 91f24ae (refactor: update ServerRuntime to include GameServer and improve method signatures)
        }
    }
}
