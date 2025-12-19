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
    }

    ClientRuntime::~ClientRuntime()
    {
        stop();
        _client.reset();
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

    void ClientRuntime::runDisplay()
    {
        using clock = std::chrono::steady_clock;
        auto last = clock::now();

        while (_running) {
            auto now = clock::now();
            const float dt = std::chrono::duration<float>(now - last).count();
            last = now;

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