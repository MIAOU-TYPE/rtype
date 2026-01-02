/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPServer
*/

#include "TCPServer.hpp"

namespace
{
    [[nodiscard]] bool wouldBlock() noexcept
    {
#ifdef _WIN32
        const int e = WSAGetLastError();
        return e == WSAEWOULDBLOCK;
#else
        return errno == EAGAIN || errno == EWOULDBLOCK;
#endif
    }
} // namespace

namespace Net::Server
{

    TCPServer::TCPServer(const std::shared_ptr<NetWrapper> &wrapper)
        : _netWrapper(wrapper), _packet(std::make_shared<TCPPacket>(64 * 1024))
    {
    }

    TCPServer::~TCPServer()
    {
        TCPServer::stop();
    }

    void TCPServer::setNonBlocking(const bool nonBlocking)
    {
        _nonBlocking = nonBlocking;

        if (_listenFd != kInvalidSocket)
            (void) _netWrapper->setNonBlocking(_listenFd, _nonBlocking);

        std::scoped_lock lock(_mutex);
        for (const auto &sock : _clients | std::views::keys)
            (void) _netWrapper->setNonBlocking(sock, _nonBlocking);
    }

    void TCPServer::start()
    {
        try {
            if (!AServer::isStoredIpCorrect() || !AServer::isStoredPortCorrect())
                throw ServerError("{TCPServer::start} Invalid IP or Port");

            if (_netWrapper->initNetwork() != 0)
                throw ServerError("{TCPServer::start} Network initialization failed");

            if (_listenFd = _netWrapper->socket(AF_INET, SOCK_STREAM, 0); _listenFd == kInvalidSocket)
                throw ServerError("{TCPServer::start} Socket creation failed");

            constexpr int yes = 1;
            if (const auto result = _netWrapper->setSocketOpt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
                result != 0)
                throw ServerError("{TCPServer::start} setsockopt(SO_REUSEADDR) failed");

            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(static_cast<uint16_t>(_port));
            if (::inet_pton(AF_INET, _ip.c_str(), &addr.sin_addr) != 1)
                throw ServerError("{TCPServer::start} Invalid IP address format");

            if (_netWrapper->bind(_listenFd, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) != 0)
                throw ServerError("{TCPServer::start}");

            if (_netWrapper->listen(_listenFd, 64) != 0)
                throw ServerError("{TCPServer::start} listen failed");

            setNonBlocking(true);
        } catch (...) {
            stop();
            throw;
        }
        AServer::setRunning(true);
        std::cout << "{TCPServer::start} Server started on " << _ip << ":" << _port << std::endl;
    }

    void TCPServer::stop() noexcept
    {
        try {
            if (!_isRunning.exchange(false))
                return;

            {
                std::scoped_lock lock(_mutex);
                for (const auto &sock : _clients | std::views::keys)
                    _netWrapper->closeSocket(sock);
                _clients.clear();
                _endpointToFd.clear();
            }

            if (_listenFd != kInvalidSocket) {
                _netWrapper->closeSocket(_listenFd);
                _listenFd = kInvalidSocket;
            }

            (void) _netWrapper->cleanupNetwork();
        } catch (...) {
            std::cerr << "{TCPServer::~TCPServer} Exception during stop()" << std::endl;
        }
    }

    void TCPServer::readPackets() noexcept
    {
        if (!_isRunning.load())
            return;

        if (!_nonBlocking)
            return;

        acceptLoop();
        readClients();
    }

    void TCPServer::acceptLoop()
    {
        while (true) {
            sockaddr_in clientAddr{};
            socklen_t len = sizeof(clientAddr);
            socketHandle clientFd = _netWrapper->accept(_listenFd, reinterpret_cast<sockaddr *>(&clientAddr), &len);

            if (clientFd == kInvalidSocket) {
                if (wouldBlock())
                    break;
                std::cerr << "{TCPServer::acceptLoop} accept socket connection failed" << std::endl;
                break;
            }

            if (_netWrapper->setNonBlocking(clientFd, _nonBlocking) != 0) {
                _netWrapper->closeSocket(clientFd);
                continue;
            }

            {
                const AddressKey key = {clientAddr.sin_addr.s_addr, clientAddr.sin_port};
                std::scoped_lock lock(_mutex);
                _clients.emplace(clientFd, std::make_unique<ClientState>(clientAddr));
                _endpointToFd[key] = clientFd;
            }
        }
    }

    void TCPServer::dropClient(const socketHandle clientFd)
    {
        {
            sockaddr_in addr{};
            std::scoped_lock lock(_mutex);
            const auto it = _clients.find(clientFd);
            if (it == _clients.end())
                return;
            addr = it->second->addr;
            _clients.erase(it);
            _endpointToFd.erase({addr.sin_addr.s_addr, addr.sin_port});
        }
        _netWrapper->closeSocket(clientFd);
    }

    std::vector<socketHandle> TCPServer::snapshotClientSockets() const
    {
        std::vector<socketHandle> sockets;
        std::scoped_lock lock(_mutex);

        sockets.reserve(_clients.size());
        for (const auto &sock : _clients | std::views::keys)
            sockets.push_back(sock);

        return sockets;
    }

    void TCPServer::readOneClient(const socketHandle clientFd, std::array<uint8_t, 4096> &tmp)
    {
        while (true) {
            const auto received = _netWrapper->recv(clientFd, tmp.data(), tmp.size(), 0);

            if (received == 0) {
                dropClient(clientFd);
                return;
            }

            if (received < 0) {
                if (wouldBlock())
                    return;
                dropClient(clientFd);
                return;
            }

            if (!onBytesReceived(clientFd, tmp.data(), static_cast<size_t>(received)))
                return;
        }
    }

    bool TCPServer::extractPayloads(
        const socketHandle clientFd, const uint8_t *data, const size_t len, std::vector<PendingPayload> &out) noexcept
    {
        const auto it = _clients.find(clientFd);
        if (it == _clients.end())
            return false;

        const auto &client = it->second;

        if (!client->rx.write(data, len))
            return false;

        while (client->rx.readable() >= 4) {
            uint32_t size = 0;
            if (!client->rx.peek(reinterpret_cast<uint8_t *>(&size), 4))
                break;

            size = ntohl(size);
            if (size == 0 || size > MAXSIZE)
                return false;

            if (client->rx.readable() < 4 + size)
                break;

            client->rx.read(nullptr, 4);

            PendingPayload p;
            p.addr = client->addr;
            p.bytes.resize(size);
            client->rx.read(p.bytes.data(), size);

            out.push_back(std::move(p));
        }

        return true;
    }

    void TCPServer::enqueuePayloads(const std::vector<PendingPayload> &payloads) noexcept
    {
        std::scoped_lock lock(_queueMutex);

        for (const auto &[addr, bytes] : payloads) {
            auto pkt = _packet->newPacket();
            pkt->setAddress(addr);
            pkt->setSize(static_cast<uint32_t>(bytes.size()));
            std::memcpy(pkt->buffer(), bytes.data(), bytes.size());
            _queue.push(std::move(pkt));
        }
    }

    bool TCPServer::onBytesReceived(socketHandle clientFd, const uint8_t *data, const size_t len) noexcept
    {
        std::vector<PendingPayload> ready;
        ready.reserve(4);

        {
            std::scoped_lock lock(_mutex);
            if (!extractPayloads(clientFd, data, len, ready)) {
                dropClient(clientFd);
                return false;
            }
        }

        if (!ready.empty())
            enqueuePayloads(ready);

        return true;
    }

    void TCPServer::readClients() noexcept
    {
        const auto clientSockets = snapshotClientSockets();

        std::array<uint8_t, 4096> tmp{};

        for (const socketHandle clientFd : clientSockets) {
            readOneClient(clientFd, tmp);
            flushWrites(clientFd);
        }
    }

    void TCPServer::flushWrites(const socketHandle clientFd) noexcept
    {
        bool mustDrop = false;

        {
            std::scoped_lock lock(_mutex);
            const auto it = _clients.find(clientFd);
            if (it == _clients.end())
                return;

            const auto &client = it->second;

            while (client->tx.readable() > 0) {
                uint8_t tmp[4096];
                const size_t toSend = (std::min) (client->tx.readable(), sizeof(tmp));
                client->tx.peek(tmp, toSend);

                const auto size = _netWrapper->send(clientFd, tmp, toSend, 0);

                if (size < 0) {
                    if (wouldBlock())
                        return;
                    mustDrop = true;
                    break;
                }
                if (size == 0)
                    return;

                client->tx.read(nullptr, static_cast<size_t>(size));
            }
        }

        if (mustDrop)
            dropClient(clientFd);
    }

    bool TCPServer::popPacket(std::shared_ptr<IPacket> &pkt) noexcept
    {
        std::scoped_lock lock(_queueMutex);
        if (_queue.empty())
            return false;
        pkt = std::move(_queue.front());
        _queue.pop();
        return true;
    }

    bool TCPServer::sendPacket(const IPacket &pkt) noexcept
    {
        const auto addr = pkt.address();
        if (!addr)
            return false;

        std::scoped_lock lock(_mutex);

        const auto it = _endpointToFd.find({addr->sin_addr.s_addr, addr->sin_port});
        if (it == _endpointToFd.end())
            return false;

        const auto &client = _clients.at(it->second);

        const auto size = static_cast<uint32_t>(pkt.size());
        if (size == 0 || size > MAXSIZE)
            return false;

        uint32_t beSize = htonl(size);

        if (!client->tx.write(reinterpret_cast<uint8_t *>(&beSize), 4))
            return false;

        if (!client->tx.write(pkt.buffer(), size))
            return false;

        return true;
    }

} // namespace Net::Server
