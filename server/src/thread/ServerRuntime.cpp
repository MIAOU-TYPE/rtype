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
    _roomManager =
        std::make_shared<Engine::RoomManager>(_sessionManager, _server, _packetFactory, "levels/level1.json");

    _packetRouter = std::make_shared<PacketRouter>(_sessionManager, _roomManager);
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
    _roomManager->forEachRoom([](Engine::Room &room) {
        room.stop();
    });
    if (_snapshotThread.joinable())
        _snapshotThread.join();
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
    while (_server->isRunning()) {
        if (std::shared_ptr<IPacket> pkt = nullptr; _server->popPacket(pkt)) {
            _packetRouter->handlePacket(pkt);
        }
    }
}

void ServerRuntime::runSnapshot() const
{
    using clock = std::chrono::steady_clock;
    constexpr auto Tick = std::chrono::milliseconds(50);
    auto nextTick = clock::now();
    std::vector<SnapshotEntity> entities;

    while (_running) {
        std::this_thread::sleep_until(nextTick);
        nextTick += Tick;

        _roomManager->forEachRoom([&](const Engine::Room &room) {
            room.gameServer().buildSnapshot(entities);

            if (entities.empty())
                return;

            if (const auto basePacket = _packetFactory->createSnapshotPacket(entities)) {
                for (const int sessionId : room.sessions()) {
                    if (const sockaddr_in *addr = _sessionManager->getAddress(sessionId)) {
                        auto packet = basePacket->clone();
                        packet->setAddress(*addr);
                        _server->sendPacket(*packet);
                    }
                }
            }
        });

        if (auto now = clock::now(); now > nextTick + Tick)
            nextTick = now;
    }
}
