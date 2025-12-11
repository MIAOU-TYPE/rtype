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
    _sessionManager = std::make_shared<Server::SessionManager>();
    _gameServer = std::make_shared<Game::GameServer>(_sessionManager, _server);
    _packetRouter = std::make_shared<PacketRouter>(_sessionManager, _gameServer);
}

ServerRuntime::~ServerRuntime()
{
    std::lock_guard lock(_mutex);
    if (!_stopRequested)
        stop();
    _server = nullptr;
}

void ServerRuntime::wait()
{
    std::unique_lock lock(_mutex);
    _cv.wait(lock, [this]() {
        return _stopRequested;
    });
}

void ServerRuntime::start()
{
    _server->start();
    _running = true;
    _receiverThread = std::thread(&ServerRuntime::runReceiver, this);
    _processorThread = std::thread(&ServerRuntime::runProcessor, this);
    _updateThread = std::thread(&ServerRuntime::runUpdate, this);
}

void ServerRuntime::stop()
{
    {
        std::lock_guard lock(_mutex);
        _stopRequested = true;
        _running = false;
    }
    _cv.notify_all();

    _server->setRunning(false);
    if (_receiverThread.joinable())
        _receiverThread.join();
    if (_processorThread.joinable())
        _processorThread.join();
    if (_updateThread.joinable())
        _updateThread.join();
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
    while (_server->isRunning()) {
        if (std::shared_ptr<IPacket> pkt = nullptr; _server->popPacket(pkt)) {
            _packetRouter->handlePacket(pkt);
        }
    }
}

void ServerRuntime::runUpdate() const
{
    while (_running) {
        _gameServer->tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
