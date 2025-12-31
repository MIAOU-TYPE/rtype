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
} // namespace

namespace Thread
{
    ClientRuntime::ClientRuntime(
        const std::shared_ptr<Graphics::IGraphics> &graphics, const std::shared_ptr<Network::INetClient> &client)
        : _client(client), _graphics(graphics), _packetFactory(client->getTemplatedPacket())
    {
        _graphics->create(Graphics::Extent2u{1280, 720}, "R-Type", false);
        _renderer = _graphics->createRenderer();
        _eventBus = std::make_shared<Engine::EventBus>();
        _eventRegistry = std::make_unique<Engine::EventRegistry>(_eventBus);
        _packetRouter = std::make_unique<Ecs::PacketRouter>(std::make_shared<Ecs::ClientController>(_commandBuffer));

        _input = std::make_unique<Engine::InputState>();
        _spriteRegistry = std::make_shared<Engine::SpriteRegistry>();
        _world = std::make_unique<World::ClientWorld>(_spriteRegistry);
        _stateManager = std::make_unique<Engine::StateManager>();
        _stateManager->changeState(std::make_unique<Engine::MenuState>(_graphics, _renderer));
        Utils::AssetLoader::load(_renderer->textures(), _spriteRegistry);
    }

    ClientRuntime::~ClientRuntime()
    {
        stop();
        _client.reset();
        _graphics.reset();
    }

    void ClientRuntime::start()
    {
        try {
            _client->start();
        } catch (const std::exception &e) {
            stop();
            std::cerr << "Failed to start client: " << e.what() << std::endl;
            return;
        }
        _running = true;
        _client->sendPacket(*_packetFactory.makeBase(Net::Protocol::UDP::CONNECT));
        setupEventsRegistry();
        _receiverThread = std::thread(&ClientRuntime::runReceiver, this);
        _updaterThread = std::thread(&ClientRuntime::runUpdater, this);
    }

    void ClientRuntime::stop()
    {
        if (_stopRequested.exchange(true))
            return;

        _running = false;
        _cv.notify_all();
        _client->sendPacket(*_packetFactory.makeBase(Net::Protocol::UDP::DISCONNECT));

        if (_receiverThread.joinable())
            _receiverThread.join();
        if (_updaterThread.joinable())
            _updaterThread.join();
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

    void ClientRuntime::runDisplay()
    {
        constexpr auto Tick = std::chrono::milliseconds(16);

        auto nextTick = clock::now();
        std::shared_ptr<const std::vector<Engine::RenderCommand>> localRenderCommands;

        while (_running && _stateManager->isRunning()) {
            nextTick += Tick;

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
            _client->receivePackets();
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

        _writeRenderCommands = std::make_shared<std::vector<Engine::RenderCommand>>();

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
        _eventRegistry->onKeyPressed(Engine::Key::Up, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{true, false, false, false, false}));
        });

        _eventRegistry->onKeyPressed(Engine::Key::Down, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, true, false, false, false}));
        });

        _eventRegistry->onKeyPressed(Engine::Key::Left, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, false, true, false, false}));
        });

        _eventRegistry->onKeyPressed(Engine::Key::Right, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, false, false, true, false}));
        });

        _eventRegistry->onKeyReleased(Engine::Key::Space, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, false, false, false, true}));
        });

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
    }

    void ClientRuntime::processNetworkPackets(const steadyClock::time_point deadline, const int maxPackets) const
    {
        int processedPacket = 0;

        while (processedPacket < maxPackets && clock::now() < deadline) {
            std::shared_ptr<Net::IPacket> pkt;
            if (!_client->popPacket(pkt))
                break;

            _packetRouter->handlePacket(pkt);
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

        Engine::RenderSystem::update(_world->registry(), _spriteRegistry, *_writeRenderCommands);

        {
            std::scoped_lock lock(_frameMutex);
            _readRenderCommands = _writeRenderCommands;
        }
    }

} // namespace Thread
