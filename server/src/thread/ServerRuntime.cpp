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
    if (!_server)
        throw ThreadError("{ServerRuntime::ServerRuntime} Invalid server pointer");
    _packetFactory = std::make_shared<Factory::PacketFactory>(std::make_shared<Net::UDPPacket>());
    _sessionManager = std::make_shared<Server::SessionManager>();
    _gameServer = std::make_shared<Game::GameServer>(_sessionManager, _server, _packetFactory);
    _packetRouter = std::make_shared<PacketRouter>(_sessionManager, _gameServer);
}

ServerRuntime::~ServerRuntime()
{
    std::scoped_lock lock(_mutex);
    if (!_stopRequested)
        stop();
    _server = nullptr;
}

void ServerRuntime::wait()
{
    std::unique_lock lock(_mutex);
    _cv.wait(lock, [this]() {
        return _stopRequested.load();
    });
}

void ServerRuntime::start()
{
    try {
        _server->start();
    } catch (std::exception &e) {
        throw ThreadError(std::string("{ServerRuntime::start} Failed to start server: ") + e.what());
    }
    _running = true;
    _receiverThread = std::thread(&ServerRuntime::runReceiver, this);
    _processorThread = std::thread(&ServerRuntime::runProcessor, this);
    _updateThread = std::thread(&ServerRuntime::runUpdate, this);
    _snapshotThread = std::thread(&ServerRuntime::runSnapshot, this);
}

void ServerRuntime::stop()
{
    {
        std::scoped_lock lock(_mutex);
        _stopRequested = true;
        _running = false;
    }
    _cv.notify_all();

    _server->setRunning(false);
    if (_snapshotThread.joinable())
        _snapshotThread.join();
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
    using clock = std::chrono::steady_clock;
    constexpr auto Tick = std::chrono::milliseconds(16);
    auto next = clock::now();

    while (_running) {
        next += Tick;

        _gameServer->tick();

        std::this_thread::sleep_until(next);

        if (const auto now = clock::now(); now > next + 5 * Tick)
            next = now;
    }
}

void ServerRuntime::runSnapshot() const
{
    std::vector<SnapshotEntity> entities;

    using clock = std::chrono::steady_clock;
    constexpr auto Tick = std::chrono::milliseconds(50);
    auto nextTick = clock::now();

    while (_running) {
        std::this_thread::sleep_until(nextTick);
        nextTick += Tick;

        _gameServer->buildSnapshot(entities);

        if (!entities.empty()) {
            if (auto basePacket = _packetFactory->createSnapshotPacket(entities)) {
                _sessionManager->forEachSession([&](int, const sockaddr_in &addr) {
                    const auto packet = basePacket->clone();
                    if (!packet)
                        return;
                    packet->setAddress(addr);
                    _server->sendPacket(*packet);
                });
            }
        }

        if (auto now = clock::now(); now > nextTick + Tick)
            nextTick = now;
    }
}
