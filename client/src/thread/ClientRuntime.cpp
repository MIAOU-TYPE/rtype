/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ClientRuntime
*/

#include "ClientRuntime.hpp"

namespace Thread
{
    using clock = std::chrono::steady_clock;

    ClientRuntime::ClientRuntime(
        const std::shared_ptr<Graphics::IGraphics> &graphics, const std::shared_ptr<Network::INetClient> &client)
        : _client(client), _graphics(graphics), _packetFactory(client->getTemplatedPacket())
    {
        _graphics->create(800, 600, "R-Type", false);
        _renderer = _graphics->createRenderer();
        _eventBus = std::make_shared<Core::EventBus>();
        _eventRegistry = std::make_unique<Core::EventRegistry>(_eventBus);
        _packetRouter = std::make_unique<Ecs::PacketRouter>(std::make_shared<Ecs::ClientController>(_commandBuffer));

        _spriteRegistry = std::make_shared<Engine::SpriteRegistry>();
        _world = std::make_unique<Engine::ClientWorld>(_spriteRegistry);
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
        _client->sendPacket(*_packetFactory.makeBase(Net::Protocol::CONNECT));
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
        _client->sendPacket(*_packetFactory.makeBase(Net::Protocol::DISCONNECT));

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

    std::shared_ptr<Core::EventBus> ClientRuntime::getEventBus() const noexcept
    {
        return _eventBus;
    }

    void ClientRuntime::runDisplay()
    {
        constexpr auto Tick = std::chrono::milliseconds(16);

        auto nextTick = clock::now();

        while (_running) {
            nextTick += Tick;

            _graphics->pollEvents(*_eventBus);
            _eventBus->dispatch();

            std::shared_ptr<const std::vector<Engine::RenderCommand>> renderCmds;
            {
                std::scoped_lock lock(_frameMutex);
                renderCmds = _frontCmds;
            }

            _renderer->beginFrame();
            if (renderCmds) {
                for (const auto &cmd : *renderCmds)
                    _renderer->draw(cmd);
            }
            _renderer->endFrame();

            std::this_thread::sleep_until(nextTick);

            if (auto after = clock::now(); after > nextTick + 2 * Tick)
                nextTick = after;
        }
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
        static constexpr float MaxFrameDt = 0.25f;
        static constexpr int MaxStepsPerTick = 4;

        std::vector<Engine::RenderCommand> built;
        built.reserve(2048);

        auto nextTick = clock::now();
        auto last = nextTick;
        float accumulator = 0.f;

        _backCmds = std::make_shared<std::vector<Engine::RenderCommand>>();

        while (_running) {
            nextTick += Tick;

            const auto tickStart = clock::now();
            const auto deadline = tickStart + Tick - std::chrono::milliseconds(1);

            const auto now = tickStart;
            float frameDt = std::chrono::duration<float>(now - last).count();
            last = now;
            if (frameDt > MaxFrameDt)
                frameDt = MaxFrameDt;
            accumulator += frameDt;

            int pkts = 0;
            while (pkts < 256 && clock::now() < deadline) {
                std::shared_ptr<Net::IPacket> pkt;
                if (!_client->popPacket(pkt))
                    break;
                _packetRouter->handlePacket(pkt);
                ++pkts;
            }

            int applied = 0;
            Engine::WorldCommand cmd;
            while (applied < 5000 && clock::now() < deadline && _commandBuffer.tryPop(cmd)) {
                _world->applyCommand(cmd);
                ++applied;
            }

            int steps = 0;
            while (accumulator >= FixedDt && steps < MaxStepsPerTick && clock::now() < deadline) {
                _world->step(FixedDt);
                accumulator -= FixedDt;
                ++steps;
            }
            if (steps == MaxStepsPerTick && accumulator > FixedDt) {
                accumulator = 0.f;
            }

            _backCmds->clear();
            Engine::RenderSystem::update(_world->registry(), _spriteRegistry, *_backCmds);
            {
                std::scoped_lock lock(_frameMutex);
                _frontCmds = _backCmds;
                _backCmds = std::make_shared<std::vector<Engine::RenderCommand>>();
            }

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
        _eventRegistry->onKeyPressed(Core::Key::Up, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{true, false, false, false, false}));
        });

        _eventRegistry->onKeyPressed(Core::Key::Down, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, true, false, false, false}));
        });

        _eventRegistry->onKeyPressed(Core::Key::Left, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, false, true, false, false}));
        });

        _eventRegistry->onKeyPressed(Core::Key::Right, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, false, false, true, false}));
        });

        _eventRegistry->onKeyPressed(Core::Key::Space, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, false, false, false, true}));
        });
    }
} // namespace Thread
