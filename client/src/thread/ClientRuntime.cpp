/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ClientRuntime
*/

#include "ClientRuntime.hpp"

namespace
{
    using clock = std::chrono::steady_clock;

    void syncToNextTick(clock::time_point &nextTick, const std::chrono::milliseconds maxDrift)
    {
        std::this_thread::sleep_until(nextTick);

        if (const auto now = clock::now(); now > nextTick + maxDrift)
            nextTick = now;
    }

    uint32_t nextReqId()
    {
        static std::atomic_uint32_t g{1};
        return g.fetch_add(1, std::memory_order_relaxed);
    }
} // namespace

namespace Thread
{
    ClientRuntime::ClientRuntime(const std::shared_ptr<Graphics::IGraphics> &graphics,
        const std::shared_ptr<Network::INetClient> &udpClient, const std::shared_ptr<Network::INetClient> &tcpClient)
        : _graphics(graphics), _udpClient(udpClient), _udpPacketFactory(udpClient->getTemplatedPacket()),
          _tcpClient(tcpClient), _tcpPacketFactory(tcpClient->getTemplatedPacket())
    {
        _graphics->create(Graphics::Extent2u{1280, 720}, "R-Type", false);
        _renderer = _graphics->createRenderer();
        _eventBus = std::make_shared<Engine::EventBus>();
        _eventRegistry = std::make_unique<Engine::EventRegistry>(_eventBus);
        _udpPacketRouter =
            std::make_unique<Ecs::UDPPacketRouter>(std::make_shared<Ecs::ClientController>(_commandBuffer));
        _tcpPacketRouter = std::make_unique<Network::TCPPacketRouter>();
        _input = std::make_unique<Engine::InputState>();
        _spriteRegistry = std::make_shared<Engine::SpriteRegistry>();
        _world = std::make_shared<World::ClientWorld>(_spriteRegistry);
        _stateManager = std::make_unique<Engine::StateManager>();
        _authCtx = std::make_shared<Engine::AuthContext>();

        _musicRegistry = std::make_shared<Engine::MusicRegistry>(_renderer->musics());
        _soundRegistry = std::make_shared<Engine::SoundRegistry>(_renderer->sounds());

        _roomManager = std::make_shared<Engine::RoomManager>(_graphics->resources());
        _stateManager->changeState(std::make_unique<Engine::MenuState>(
            _graphics, _renderer, _musicRegistry, _soundRegistry, _roomManager, _eventBus, _authCtx));
        _readRenderCommands = std::make_shared<std::vector<Engine::RenderCommand>>();
        _writeRenderCommands = std::make_shared<std::vector<Engine::RenderCommand>>();
        Utils::AssetLoader::load(_renderer->textures(), _spriteRegistry);
    }

    ClientRuntime::~ClientRuntime()
    {
        try {
            stop();
            _udpClient.reset();
            _graphics.reset();
        } catch (...) {
            std::cerr << "{ClientRuntime::~ClientRuntime} Exception during destruction" << std::endl;
        }
    }

    void ClientRuntime::start()
    {
        try {
            _tcpClient->start();
            _udpClient->start();
        } catch (...) {
            stop();
            throw;
        }
        _running = true;
        applyLoadedSettings();
        setupGlobalEventHandlers();
        setupEventsRegistry();
        _tcpThread = std::thread(&ClientRuntime::runTcp, this);
        _receiverThread = std::thread(&ClientRuntime::runReceiver, this);
        _updaterThread = std::thread(&ClientRuntime::runUpdater, this);
    }

    void ClientRuntime::applyLoadedSettings() const noexcept
    {
        const auto &config = Utils::SettingsConfig::getInstance();

        const float musicVolume = config.isMusicMuted() ? 0.f : static_cast<float>(config.getMusicVolume());
        _musicRegistry->setMusicVolume(musicVolume);
        if (config.isMusicMuted())
            _musicRegistry->setVolumeBeforeMute(static_cast<float>(config.getMusicVolume()));

        const float sfxVolume = config.isSfxMuted() ? 0.f : static_cast<float>(config.getSfxVolume());
        _soundRegistry->setSoundVolume(sfxVolume);
        if (config.isSfxMuted())
            _soundRegistry->setVolumeBeforeMute(static_cast<float>(config.getSfxVolume()));

        _graphics->setResolution(config.getResolution());

        _renderer->setColorBlindMode(config.getColorBlindMode());
    }

    void ClientRuntime::stop()
    {
        if (_stopRequested.exchange(true))
            return;

        _running = false;
        _cv.notify_all();
        if (const auto leavePkt = _tcpPacketFactory.makeLeaveRoom(nextReqId()))
            (void) _tcpClient->sendPacket(*leavePkt);
        if (const auto discoPkt = _udpPacketFactory.makeBase(Net::Protocol::UDP::DISCONNECT))
            (void) _udpClient->sendPacket(*discoPkt);

        if (_tcpThread.joinable())
            _tcpThread.join();
        if (_receiverThread.joinable())
            _receiverThread.join();
        if (_updaterThread.joinable())
            _updaterThread.join();
        _tcpClient->close();
        _udpClient->close();
    }

    void ClientRuntime::wait()
    {
        std::unique_lock lock(_mutex);
        _cv.wait(lock, [this]() {
            return _stopRequested.load();
        });
    }

    std::shared_ptr<Engine::EventBus> ClientRuntime::getEventBus() const noexcept
    {
        return _eventBus;
    }

    void ClientRuntime::rebindControls() const
    {
        _eventRegistry->clear();
        setupEventsRegistry();
    }

    void ClientRuntime::runDisplay()
    {
        constexpr auto Tick = std::chrono::milliseconds(16);

        auto nextTick = clock::now();
        std::shared_ptr<const std::vector<Engine::RenderCommand>> localRenderCommands;

        while (_running && _stateManager->isRunning()) {
            nextTick += Tick;

            if (Utils::SettingsConfig::getInstance().needsRebind()) {
                rebindControls();
                Utils::SettingsConfig::getInstance().clearRebindFlag();
            }

            if (_pendingGameStart.exchange(false, std::memory_order_acq_rel)) {
                try {
                    std::weak_ptr<World::ClientWorld> w = _world;
                    _stateManager->changeState(
                        std::make_unique<Engine::GameState>(_musicRegistry, _soundRegistry, _renderer, [w]() {
                            if (auto s = w.lock())
                                return static_cast<int>(s->getScore());
                            return 0;
                        }));
                } catch (...) {
                    std::cerr << "{ClientRuntime::runDisplay} unknown exception\n";
                }
            }
            if (_pendingAuthOk.exchange(false, std::memory_order_acq_rel)) {
                try {
                    _stateManager->changeState(std::make_unique<Engine::MenuState>(
                        _graphics, _renderer, _musicRegistry, _soundRegistry, _roomManager, _eventBus, _authCtx));
                } catch (...) {
                    std::cerr << "{ClientRuntime::runDisplay} state change failed\n";
                }
            }
            _graphics->pollEvents(*_eventBus);
            _eventBus->dispatch();
            _stateManager->update(_input->consumeFrame());

            {
                std::scoped_lock lock(_frameMutex);
                localRenderCommands = _readRenderCommands;
            }
            _renderer->beginFrame();
            if (localRenderCommands) {
                for (const auto &cmd : *localRenderCommands)
                    _renderer->draw(cmd);
            }
            _stateManager->render();
            _renderer->endFrame();
            syncToNextTick(nextTick, Tick * 2);
        }
        _graphics->close();
        stop();
    }

    void ClientRuntime::runReceiver() const
    {
        while (_running) {
            _udpClient->receivePackets();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void ClientRuntime::runUpdater()
    {
        constexpr auto Tick = std::chrono::milliseconds(16);
        constexpr float FixedDt = 1.0f / 60.0f;
        static constexpr float MaxFrameDt = 0.16f;
        static constexpr int MaxStepsPerTick = 4;

        auto nextTick = clock::now();
        auto last = nextTick;
        float accumulator = 0.f;

        while (_running) {
            nextTick += Tick;

            const auto now = clock::now();
            const auto deadline = now + Tick - std::chrono::milliseconds(1);

            float frameDt = std::chrono::duration<float>(now - last).count();
            last = now;
            if (frameDt > MaxFrameDt)
                frameDt = MaxFrameDt;
            accumulator += frameDt;

            processNetworkPackets(deadline, 256);
            applyWorldCommands(deadline, 500);

            _world->updateInterpolatedPositions();

            int steps = 0;
            while (accumulator >= FixedDt && steps < MaxStepsPerTick && clock::now() < deadline) {
                _world->step(FixedDt);
                accumulator -= FixedDt;
                steps++;
            }
            if (steps == MaxStepsPerTick && accumulator > FixedDt)
                accumulator = 0.f;

            buildAndSwapRenderCommands();

            std::this_thread::sleep_until(nextTick);

            if (const auto after = clock::now(); after > nextTick + 2 * Tick) {
                nextTick = after;
                last = after;
                accumulator = 0.f;
            }
        }
    }

    void ClientRuntime::setupEventsRegistry() const
    {
        const auto [up, down, left, right, shoot] = Utils::SettingsConfig::getInstance().getMovementKeys();

        _eventRegistry->onKeyPressed(up, [this]() {
            _udpClient->sendPacket(*_udpPacketFactory.makeInput(PlayerInput{true, false, false, false, false}));
        });

        _eventRegistry->onKeyPressed(down, [this]() {
            _udpClient->sendPacket(*_udpPacketFactory.makeInput(PlayerInput{false, true, false, false, false}));
        });

        _eventRegistry->onKeyPressed(left, [this]() {
            _udpClient->sendPacket(*_udpPacketFactory.makeInput(PlayerInput{false, false, true, false, false}));
        });

        _eventRegistry->onKeyPressed(right, [this]() {
            _udpClient->sendPacket(*_udpPacketFactory.makeInput(PlayerInput{false, false, false, true, false}));
        });

        _eventRegistry->onKeyReleased(shoot, [this]() {
            _udpClient->sendPacket(*_udpPacketFactory.makeInput(PlayerInput{false, false, false, false, true}));
        });
    }

    void ClientRuntime::setupGlobalEventHandlers()
    {
        _eventBus->on<Engine::KeyPressed>([this](const Engine::KeyPressed &e) {
            _input->setKeyPressed(e.key);
        });

        _eventBus->on<Engine::KeyReleased>([this](const Engine::KeyReleased &e) {
            _input->setKeyReleased(e.key);
        });

        _eventBus->on<Engine::MouseMoved>([this](const Engine::MouseMoved &e) {
            _input->setMouse(static_cast<float>(e.posX), static_cast<float>(e.posY));
        });

        _eventBus->on<Engine::MousePressed>([this](const Engine::MousePressed &e) {
            _input->setMouse(static_cast<float>(e.posX), static_cast<float>(e.posY));
            _input->setMousePressed();
        });

        _eventBus->on<Engine::MouseReleased>([this](const Engine::MouseReleased &e) {
            _input->setMouse(static_cast<float>(e.posX), static_cast<float>(e.posY));
            _input->setMouseReleased();
        });

        _eventBus->on<Engine::CreateRoomRequested>([this](const Engine::CreateRoomRequested &e) {
            const auto req = nextReqId();
            _tcpClient->sendPacket(*_tcpPacketFactory.makeCreateRoom(req, e.roomName, e.maxPlayers));
        });

        _eventBus->on<Engine::JoinRoomRequested>([this](const Engine::JoinRoomRequested &e) {
            const auto req = nextReqId();
            _tcpClient->sendPacket(*_tcpPacketFactory.makeJoinRoom(req, e.roomId));
        });

        _eventBus->on<Engine::ListRoomRequested>([this](const Engine::ListRoomRequested &) {
            _tcpClient->sendPacket(*_tcpPacketFactory.makeListRooms(11));
        });

        _eventBus->on<Engine::AuthRegisterRequested>([this](const Engine::AuthRegisterRequested &e) {
            const auto req = nextReqId();
            _lastAuthReq.store(req, std::memory_order_release);
            _tcpClient->sendPacket(*_tcpPacketFactory.makeAuthRegister(req, e.username, e.password));
        });

        _eventBus->on<Engine::AuthLoginRequested>([this](const Engine::AuthLoginRequested &e) {
            const auto req = nextReqId();
            _lastAuthReq.store(req, std::memory_order_release);
            _tcpClient->sendPacket(*_tcpPacketFactory.makeAuthLogin(req, e.username, e.password));
        });
    }

    void ClientRuntime::processNetworkPackets(const steadyClock::time_point deadline, const int maxPackets) const
    {
        int processedPacket = 0;

        while (processedPacket < maxPackets && clock::now() < deadline) {
            std::shared_ptr<Net::IPacket> pkt;
            if (!_udpClient->popPacket(pkt))
                break;

            _udpPacketRouter->handlePacket(pkt);
            processedPacket++;
        }
    }

    void ClientRuntime::applyWorldCommands(const steadyClock::time_point deadline, const int maxCommands)
    {
        int applied = 0;
        World::WorldCommand cmd;

        while (applied < maxCommands && clock::now() < deadline && _commandBuffer.pop(cmd)) {
            _world->applyCommand(cmd);
            applied++;
        }
    }

    void ClientRuntime::buildAndSwapRenderCommands()
    {
        _writeRenderCommands->clear();
        const auto viewportSize = _renderer->getViewportSize();
        Engine::RenderSystem::update(_world->registry(), _spriteRegistry, viewportSize, *_writeRenderCommands);

        {
            std::scoped_lock lock(_frameMutex);
            std::swap(_readRenderCommands, _writeRenderCommands);
        }
    }

    void ClientRuntime::runTcp()
    {
        _tcpPacketRouter->sink()->onWelcomeSubscribe([this](uint32_t, uint16_t, uint32_t, uint16_t, uint64_t) {
            const auto ci = _tcpPacketRouter->sink()->getConnectInfo();
            if (const auto pkt = _udpPacketFactory.makeConnect(ci)) {
                _udpClient->sendPacket(*pkt);
            }
        });

        _tcpPacketRouter->sink()->onGameStartSubscribe([this](uint32_t, uint32_t) {
            _pendingGameStart.store(true, std::memory_order_release);
        });

        _tcpPacketRouter->sink()->onRoomsListSubscribe([&](uint32_t, const std::vector<RoomData> &rooms) {
            _roomManager->rooms() = rooms;
        });

        _tcpPacketRouter->sink()->onAuthOkSubscribe(
            [this](uint32_t, const uint32_t userId, const std::string_view username, const uint64_t token,
                const uint32_t ttl) {
                if (_authCtx) {
                    {
                        std::scoped_lock lk(_authCtx->m);
                        _authCtx->userId = userId;
                        _authCtx->token = token;
                        _authCtx->ttlSec = ttl;
                        _authCtx->username = std::string(username);
                        _authCtx->authError.clear();
                    }
                    _authCtx->authErrorVersion.fetch_add(1, std::memory_order_release);
                    _authCtx->authed.store(true, std::memory_order_release);
                }
                _pendingAuthOk.store(true, std::memory_order_release);
            });

        _tcpPacketRouter->sink()->onErrorSubscribe([this](const uint32_t req, uint16_t, const std::string_view msg) {
            if (const auto last = _lastAuthReq.load(std::memory_order_acquire); !last || req != last || !_authCtx)
                return;
            {
                std::scoped_lock lk(_authCtx->m);
                _authCtx->authError = std::string(msg);
            }
            _authCtx->authErrorVersion.fetch_add(1, std::memory_order_release);
        });

        auto lastHello = clock::now() - std::chrono::seconds(10);
        while (_running && _tcpClient->isRunning()) {
            _tcpClient->receivePackets();
            std::shared_ptr<Net::IPacket> pkt;
            while (_tcpClient->popPacket(pkt))
                _tcpPacketRouter->handle(pkt);
            if (const auto now = clock::now();
                !_tcpPacketRouter->sink()->isConnected() && (now - lastHello) > std::chrono::milliseconds(500)) {
                lastHello = now;
                if (const auto hello = _tcpPacketFactory.makeHello(nextReqId(), 1))
                    _tcpClient->sendPacket(*hello);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        _running.store(false, std::memory_order_release);
    }
} // namespace Thread
