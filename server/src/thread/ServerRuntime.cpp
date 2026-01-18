/*
** EPITECH PROJECT, 2025
** RType
** File description:
** ServerRuntime.cpp
*/

#include "ServerRuntime.hpp"

using namespace Net::Thread;

ServerRuntime::ServerRuntime(
    const std::shared_ptr<Server::IServer> &udpServer, const std::shared_ptr<Server::IServer> &tcpServer)
    : _udpServer(udpServer), _tcpServer(tcpServer)
{
    if (!_udpServer)
        throw ThreadError("{ServerRuntime::ServerRuntime} Invalid UDP server pointer");
    if (!_tcpServer)
        throw ThreadError("{ServerRuntime::ServerRuntime} Invalid TCP server pointer");

    try {
        std::error_code ec;
        std::filesystem::create_directories("db", ec);
        if (ec)
            throw ThreadError(std::string("{ServerRuntime} create_directories failed: ") + ec.message());
        _authDb = std::make_shared<Db::SqliteDb>("db/users.sqlite3");
        _userRepo = std::make_shared<Auth::UserStorage>(_authDb);
        _userRepo->initSchema();
        _scoreService = std::make_shared<Engine::ScoreService>(_authDb);
        _scoreService->initSchema();
        _authService = std::make_shared<Auth::AuthService>(_userRepo);
    } catch (const std::exception &e) {
        throw ThreadError(std::string("{ServerRuntime} auth init failed: ") + e.what());
    }

    _udpPacketFactory = std::make_shared<Factory::UDPPacketFactory>(std::make_shared<UDPPacket>());
    _sessionManager = std::make_shared<Server::SessionManager>();
    _roomManager = std::make_shared<Engine::RoomManager>(
        _sessionManager, _udpServer, _udpPacketFactory, _scoreService, "levels/space_level1.json");

    _udpPacketRouter = std::make_shared<UDPPacketRouter>(_sessionManager, _roomManager);

    _tcpPacketFactory = std::make_shared<Factory::TCPPacketFactory>(std::make_shared<TCPPacket>());
    _tcpPacketRouter = std::make_shared<TCPPacketRouter>(
        _sessionManager, _roomManager, _tcpServer, _tcpPacketFactory, _authService, _scoreService);
    _stopRequested.store(false);
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
        _stopRequested.store(false);
        _running.store(true);

        _adminConsole = std::make_unique<Admin::AdminConsole>(_sessionManager, _roomManager, [this]() {
            requestStop();
        });
        _adminConsole->start();
        _adminWeb = std::make_unique<Admin::AdminWebServer>(_sessionManager, _roomManager, [this]() {
            requestStop();
        });
        _adminWeb->start();
        _receiverThread = std::thread(&ServerRuntime::runReceiver, this);
        _processorThread = std::thread(&ServerRuntime::runProcessor, this);
        _snapshotThread = std::thread(&ServerRuntime::runSnapshot, this);
        _tcpThread = std::thread(&ServerRuntime::runTcp, this);
    } catch (...) {
        std::cerr << "{ServerRuntime::start} Exception during start()" << std::endl;
        requestStop();
        throw;
    }
}

void ServerRuntime::requestStop() noexcept
{
    if (bool expected = false; !_stopRequested.compare_exchange_strong(expected, true, std::memory_order_relaxed))
        return;

    _running.store(false, std::memory_order_relaxed);
    _cv.notify_all();

    _udpServer->setRunning(false);
    _tcpServer->setRunning(false);
}

void ServerRuntime::stop()
{
    requestStop();

    if (_adminConsole)
        _adminConsole->stop();

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
    using clock = std::chrono::steady_clock;
    constexpr auto Tick = std::chrono::milliseconds(16);
    auto nextTick = clock::now();

    while (_running.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_until(nextTick);
        nextTick += Tick;

        while (_udpServer->readPackets()) {}
        if (auto now = clock::now(); now > nextTick + Tick)
            nextTick = now;
    }
}

void ServerRuntime::runProcessor() const
{
    constexpr auto Tick = std::chrono::milliseconds(16);
    auto nextTick = std::chrono::steady_clock::now();

    while (_running.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_until(nextTick);
        nextTick += Tick;
        for (std::shared_ptr<IPacket> pkt = nullptr; _udpServer->popPacket(pkt);)
            _udpPacketRouter->handlePacket(pkt);
        if (auto now = std::chrono::steady_clock::now(); now > nextTick + Tick)
            nextTick = now;
    }
}

void ServerRuntime::runSnapshot() const
{
    using clock = std::chrono::steady_clock;
    constexpr auto Tick = std::chrono::milliseconds(50);
    constexpr size_t MaxUdpPacketBytes = 1200;

    auto nextTick = clock::now();
    std::vector<SnapshotEntity> entities;

    while (_running.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_until(nextTick);
        nextTick += Tick;

        const uint32_t tick = _serverTick.fetch_add(1, std::memory_order_relaxed);

        _roomManager->forEachRoom([&](const Engine::Room &room) {
            entities.clear();
            room.gameServer().buildSnapshot(entities);

            if (entities.empty())
                return;

            const auto packets = _udpPacketFactory->createSnapshotPackets(entities, tick, MaxUdpPacketBytes);
            if (packets.empty())
                return;

            for (const int sessionId : room.sessions()) {
                const sockaddr_in *addr = _sessionManager->getUdpAddress(sessionId);
                if (!addr)
                    continue;

                for (const auto &basePkt : packets) {
                    if (!basePkt)
                        continue;

                    auto pkt = basePkt->clone();
                    pkt->setAddress(*addr);
                    (void) _udpServer->sendPacket(*pkt);
                }
            }
        });

        if (auto now = clock::now(); now > nextTick + Tick)
            nextTick = now;
    }
}

void ServerRuntime::runTcp() const
{
    using clock = std::chrono::steady_clock;
    constexpr auto Tick = std::chrono::milliseconds(50);
    auto nextTick = clock::now();

    while (_running.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_until(nextTick);
        nextTick += Tick;
        (void) _tcpServer->readPackets();
        std::shared_ptr<IPacket> pkt = nullptr;
        while (_tcpServer->popPacket(pkt))
            _tcpPacketRouter->handle(pkt);
        if (auto now = clock::now(); now > nextTick + Tick)
            nextTick = now;
    }
}
