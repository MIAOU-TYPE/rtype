/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ClientRuntime
*/

#include "ClientRuntime.hpp"

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

        _spriteRegistry = std::make_shared<Engine::SpriteRegistry>();
        _world = std::make_unique<Engine::ClientWorld>(_spriteRegistry);
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

        _renderer = _graphics->createRenderer();
        _spriteRegistry = std::make_shared<Engine::SpriteRegistry>();
        _world = std::make_unique<Engine::ClientWorld>(_spriteRegistry);

        _receiverThread = std::thread(&ClientRuntime::runReceiver, this);
        _updaterThread = std::thread(&ClientRuntime::runUpdater, this);
    }

    void ClientRuntime::stop()
    {
        if (_stopRequested.exchange(true))
            return;

        _client->sendPacket(*_packetFactory.makeBase(Net::Protocol::DISCONNECT));

        _eventRegistry->onKeyReleased(Core::Key::Up, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{true, false, false, false, false}));
        });

        _eventRegistry->onKeyReleased(Core::Key::Down, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, true, false, false, false}));
        });

        _eventRegistry->onKeyReleased(Core::Key::Left, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, false, true, false, false}));
        });

        _eventRegistry->onKeyReleased(Core::Key::Right, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, false, false, true, false}));
        });

        _eventRegistry->onKeyReleased(Core::Key::Space, [this]() {
            _client->sendPacket(*_packetFactory.makeInput(PlayerInput{false, false, false, false, true}));
        });

        _running = false;
        _cv.notify_all();
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
        using clock = std::chrono::steady_clock;
        auto last = clock::now();

        while (_running) {
            auto now = clock::now();
            const float dt = std::chrono::duration<float>(now - last).count();
            last = now;

            _renderer->beginFrame();
            _world->update(dt, *_renderer);
            _renderer->endFrame();

            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }

    void ClientRuntime::runReceiver() const
    {
        while (_running) {
            _client->receivePackets();
        }
    }

    void ClientRuntime::runUpdater() const
    {
        while (_running) {
            if (std::shared_ptr<Net::IPacket> pkt; _client->popPacket(pkt)) {
                std::cout << pkt << std::endl;
            }
        }
    }
} // namespace Thread