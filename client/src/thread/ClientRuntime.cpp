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
    }

    ClientRuntime::~ClientRuntime()
    {
        std::scoped_lock lock(_mutex);
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
    }

    void ClientRuntime::runReceiver() const
    {
        while (_running) {
            _client->receivePackets();
        }
    }

} // namespace Thread