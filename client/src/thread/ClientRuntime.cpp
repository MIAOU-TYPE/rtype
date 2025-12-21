/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ClientRuntime
*/

#include "ClientRuntime.hpp"

#include <SFML/Graphics/VertexBuffer.hpp>

namespace Thread
{
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
            std::cerr << "Failed to start client: " << e.what() << std::endl;
            return;
        }
        _running = true;
        _client->sendPacket(*_packetFactory.makeBase(Net::Protocol::CONNECT));

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
            std::cout << "Key space pressed" << std::endl;
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, false, false, true, true}));
        });

        _receiverThread = std::thread(&ClientRuntime::runReceiver, this);
        _updaterThread = std::thread(&ClientRuntime::runUpdater, this);
    }

    void ClientRuntime::stop()
    {
        if (_stopRequested.exchange(true))
            return;

        _running = false;
        _client->sendPacket(*_packetFactory.makeBase(Net::Protocol::DISCONNECT));
        if (_receiverThread.joinable())
            _receiverThread.join();
        if (_updaterThread.joinable())
            _updaterThread.join();
        _cv.notify_all();
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
        while (_running) {
            _graphics->pollEvents(*_eventBus);
            _eventBus->dispatch();
            _renderer->beginFrame();

            if (_frameReady.load(std::memory_order_acquire)) {
                for (const auto& cmd : _frontFrame.commands)
                    _renderer->draw(cmd);
            }

            _renderer->endFrame();
        }
    }


    void ClientRuntime::runReceiver() const
    {
        while (_running) {
            _client->receivePackets();
        }
    }

    void ClientRuntime::runUpdater()
    {
        using clock = std::chrono::steady_clock;
        auto last = clock::now();

        while (_running) {
            Engine::WorldCommand cmd;
            auto now = clock::now();
            const float dt = std::chrono::duration<float>(now - last).count();
            last = now;

            if (std::shared_ptr<Net::IPacket> pkt; _client->popPacket(pkt))
                _packetRouter->handlePacket(pkt);
            while (_commandBuffer.tryPop(cmd))
                _world->applyCommand(cmd);
            _world->step(dt);
            _backFrame.commands.clear();
            _world->buildRenderCommands(_backFrame.commands);
            std::swap(_frontFrame, _backFrame);
            _frameReady.store(true, std::memory_order_release);
        }
    }
} // namespace Thread