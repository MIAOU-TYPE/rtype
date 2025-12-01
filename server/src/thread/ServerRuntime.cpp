/*
** EPITECH PROJECT, 2025
** RType
** File description:
** ServerRuntime.hpp
*/

#include "ServerRuntime.hpp"
#include "SessionManager.hpp"

ServerRuntime::ServerRuntime(std::shared_ptr<Server::IServer> &server) : _server(server)
{
}

ServerRuntime::~ServerRuntime()
{
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

void ServerRuntime::runReceiver()
{
    while (_server->isRunning()) {
        _server->readPackets();
    }
}

void ServerRuntime::runProcessor()
{
    SessionManager _sessionManager;

    while (_server->isRunning()) {
        std::shared_ptr<Net::IServerPacket> packet = nullptr;
        if (_server->popPacket(packet)) {
            _sessionManager.getOrCreateSession(*packet->address());
            std::cout << "Session id: " << _sessionManager.getSessionId(*packet->address()) << std::endl;
            std::cout << "{ServerRuntime} Processing packet: " << packet;
            std::cout << std::endl;
            std::flush(std::cout);
        }
    }
}
