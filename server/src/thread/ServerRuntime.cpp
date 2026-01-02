/*
** EPITECH PROJECT, 2025
** RType
** File description:
** ServerRuntime.cpp
*/

#include "ServerRuntime.hpp"

using namespace Net::Thread;

ServerRuntime::ServerRuntime(
    const std::shared_ptr<Server::IServer> &UdpServer, const std::shared_ptr<Server::IServer> &TcpServer)
    : _udpServer(UdpServer), _tcpServer(TcpServer)
{
    if (!_udpServer)
        throw ThreadError("{ServerRuntime::ServerRuntime} Invalid server pointer");
    _udpPacketFactory = std::make_shared<Factory::UDPPacketFactory>(std::make_shared<UDPPacket>());
    _sessionManager = std::make_shared<Server::SessionManager>();
    _roomManager =
        std::make_shared<Engine::RoomManager>(_sessionManager, _udpServer, _udpPacketFactory, "levels/level1.json");

    _udpPacketRouter = std::make_shared<UDPPacketRouter>(_sessionManager, _roomManager);

    _tcpPacketFactory = std::make_shared<Factory::TCPPacketFactory>(std::make_shared<TCPPacket>());
    _tcpPacketRouter = std::make_shared<TCPPacketRouter>(_sessionManager, _roomManager, _tcpServer, _tcpPacketFactory);
    _stopRequested.store(false);
}

ServerRuntime::~ServerRuntime()
{
    try {
        if (!_stopRequested.load())
            stop();
        _udpServer = nullptr;
        _tcpServer = nullptr;
    } catch (...) {
        std::cerr << "{ServerRuntime::~ServerRuntime} Exception during destruction" << std::endl;
    }
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
        _udpServer->start();
        _tcpServer->start();
    } catch (std::exception &e) {
        throw ThreadError(std::string("{ServerRuntime::start} Failed to start server: ") + e.what());
    }
    _running = true;
    _receiverThread = std::thread(&ServerRuntime::runReceiver, this);
    _processorThread = std::thread(&ServerRuntime::runProcessor, this);
    _snapshotThread = std::thread(&ServerRuntime::runSnapshot, this);
    _tcpThread = std::thread(&ServerRuntime::runTcp, this);
}

void ServerRuntime::stop()
{
    {
        std::scoped_lock lock(_mutex);
        _stopRequested.store(true);
        _running.store(false);
    }
    _cv.notify_all();

    _udpServer->setRunning(false);
    _tcpServer->setRunning(false);
    _roomManager->forEachRoom([](Engine::Room &room) {
        room.stop();
    });
    if (_snapshotThread.joinable())
        _snapshotThread.join();
    if (_receiverThread.joinable())
        _receiverThread.join();
    if (_processorThread.joinable())
        _processorThread.join();
    if (_tcpThread.joinable())
        _tcpThread.join();
    _tcpServer->stop();
    _udpServer->stop();
}

void ServerRuntime::runReceiver() const
{
    while (_udpServer->isRunning()) {
        _udpServer->readPackets();
    }
}

void ServerRuntime::runProcessor() const
{
    while (_udpServer->isRunning()) {
        if (std::shared_ptr<IPacket> pkt = nullptr; _udpServer->popPacket(pkt)) {
            _udpPacketRouter->handlePacket(pkt);
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
            entities.clear();
            room.gameServer().buildSnapshot(entities);

            if (entities.empty())
                return;

            if (const auto basePacket = _udpPacketFactory->createSnapshotPacket(entities)) {
                for (const int sessionId : room.sessions()) {
                    if (const sockaddr_in *addr = _sessionManager->getAddress(sessionId)) {
                        basePacket->setAddress(*addr);
                        _udpServer->sendPacket(*basePacket);
                    }
                }
            }
        });

        if (auto now = clock::now(); now > nextTick + Tick)
            nextTick = now;
    }
}

void ServerRuntime::runTcp() const
{
    while (_tcpServer->isRunning()) {
        _tcpServer->readPackets();
        if (std::shared_ptr<IPacket> pkt = nullptr; _tcpServer->popPacket(pkt)) {
            std::cout << "Received TCP packet of size " << pkt->size() << std::endl;
            _tcpPacketRouter->handle(pkt);
        }
    }
}
