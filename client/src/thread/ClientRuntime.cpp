/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ClientRuntime
*/

#include "ClientRuntime.hpp"

namespace Thread
{

    ClientRuntime::ClientRuntime(const std::shared_ptr<Network::NetClient> &client) : _client(client)
    {
        _entitiesFactory = std::make_shared<Ecs::EntitiesFactory>();
        _renderer = std::make_shared<Graphics::SFMLRenderer>();
        _display = std::make_shared<Display::DisplayInit>(_renderer);
        _event = std::make_shared<Events::EventInit>(_renderer);
    }

    ClientRuntime::~ClientRuntime()
    {
        if (!_stopRequested)
            stop();
        _client = nullptr;
    }

    void ClientRuntime::wait()
    {
        std::unique_lock lock(_mutex);
        _cv.wait(lock, [this]() {
            return _stopRequested;
        });
    }

    void ClientRuntime::start()
    {
        _running = true;
        _receiverThread = std::thread(&ClientRuntime::runReceiver, this);
        _updateThread = std::thread(&ClientRuntime::runUpdater, this);
        _displayThread = std::thread(&ClientRuntime::runDisplay, this);
        _eventThread = std::thread(&ClientRuntime::runEvent, this);
    }

    void ClientRuntime::stop()
    {
        {
            std::scoped_lock lock(_mutex);
            _stopRequested = true;
            _running = false;
        }
        _cv.notify_all();

        if (_receiverThread.joinable())
            _receiverThread.join();
        if (_updateThread.joinable())
            _updateThread.join();
        if (_displayThread.joinable())
            _displayThread.join();
        if (_eventThread.joinable())
            _eventThread.join();
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
            std::vector<Network::PacketData> data = _client->getAndClearPacketData();
            _entitiesFactory->parseData(data);
        }
    }

    void ClientRuntime::runDisplay()
    {
        _display->init(800, 600);

        _renderer->setActive(false);

        while (_running && _display->isWindowOpen()) {
            _renderer->setActive(true);
            _display->run();
            _renderer->setActive(false);
        }
    }

    void ClientRuntime::runEvent()
    {
        while (_running && !_event->isWindowOpen()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        while (_running && _event->isWindowOpen()) {
            _event->run();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

} // namespace Thread