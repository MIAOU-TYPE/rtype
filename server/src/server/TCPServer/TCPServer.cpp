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
    TCPServer::TCPServer(std::shared_ptr<NetWrapper> net, std::shared_ptr<IPacket> packetPrototype)
        : AServer(), _netWrapper(std::move(net)), _packetProto(std::move(packetPrototype))
    {
        if (!_netWrapper)
            throw ServerError("{TCPServer::TCPServer} requires NetWrapper");
        if (!_packetProto)
            throw ServerError("{TCPServer::TCPServer} requires a packet prototype (ex: make_shared<TcpPacket>())");
    }

    TCPServer::~TCPServer()
    {
        stop();
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
        std::cout << "[TCPServer] Starting server on " << _ip << ":" << _port << "\n";
        if (!AServer::isStoredIpCorrect())
            throw ServerError("{TCPServer::start} Invalid IP address");
        if (!isStoredPortCorrect())
            throw ServerError("{TCPServer::start} Invalid port number");

        if (_netWrapper->initNetwork() != 0)
            throw ServerError("{TCPServer::start} Network initialization failed");

        _listenFd = _netWrapper->socket(AF_INET, SOCK_STREAM, 0);
        if (_listenFd == kInvalidSocket)
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

        if (_netWrapper->setNonBlocking(_listenFd, _nonBlocking) != 0)
            throw ServerError("{TCPServer::start} setNonBlocking(listen) failed");

        AServer::setRunning(true);
    }

    void TCPServer::stop()
    {
        if (!_isRunning.exchange(false))
            return;

        {
            std::scoped_lock lock(_mutex);
            for (const auto &sock : _clients | std::views::keys)
                _netWrapper->closeSocket(sock);
            _clients.clear();
            _endpointToSock.clear();
        }

        if (_listenFd != kInvalidSocket) {
            _netWrapper->closeSocket(_listenFd);
            _listenFd = kInvalidSocket;
        }

        (void) _netWrapper->cleanupNetwork();
    }

    void TCPServer::readPackets()
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
            sockaddr_in caddr{};
            socklen_t len = sizeof(caddr);
            socketHandle c = _netWrapper->accept(_listenFd, reinterpret_cast<sockaddr *>(&caddr), &len);

            if (c == kInvalidSocket) {
                if (wouldBlock())
                    break;
                break;
            }

            if (_netWrapper->setNonBlocking(c, _nonBlocking) != 0) {
                _netWrapper->closeSocket(c);
                continue;
            }

            ClientState st;
            st.addr = caddr;
            st.rx.reserve(4096);

            {
                const EndpointKey key = keyFrom_(caddr);
                std::scoped_lock lock(_mutex);
                _clients.emplace(c, std::move(st));
                _endpointToSock[key] = c;
            }
        }
    }

    void TCPServer::dropClient(const socketHandle clientFd, const sockaddr_in &addr)
    {
        _netWrapper->closeSocket(clientFd);
        std::scoped_lock lock(_mutex);
        _clients.erase(clientFd);
        _endpointToSock.erase(keyFrom_(addr));
    }

    void TCPServer::readClients()
    {
        std::vector<socketHandle> sockets;
        {
            std::scoped_lock lock(_mutex);
            sockets.reserve(_clients.size());
            for (const auto &sock : _clients | std::views::keys)
                sockets.push_back(sock);
        }

        uint8_t tmp[4096];

        for (socketHandle s : sockets) {
            while (true) {
                const auto r = _netWrapper->recv(s, tmp, sizeof(tmp), 0);

                if (r == 0) {
                    sockaddr_in a{};
                    {
                        std::scoped_lock lock(_mutex);
                        auto it = _clients.find(s);
                        if (it == _clients.end())
                            break;
                        a = it->second.addr;
                    }
                    dropClient(s, a);
                    break;
                }

                if (r < 0) {
                    if (wouldBlock())
                        break;

                    sockaddr_in a{};
                    {
                        std::scoped_lock lock(_mutex);
                        auto it = _clients.find(s);
                        if (it == _clients.end())
                            break;
                        a = it->second.addr;
                    }
                    dropClient(s, a);
                    break;
                }

                ClientState *st = nullptr;
                {
                    std::scoped_lock lock(_mutex);
                    auto it = _clients.find(s);
                    if (it == _clients.end())
                        break;
                    st = &it->second;
                }

                st->rx.insert(st->rx.end(), tmp, tmp + static_cast<size_t>(r));

                while (st->rx.size() >= 4) {
                    const uint32_t sz = readU32BE(st->rx.data());
                    if (sz == 0 || sz > MaxFrame) {
                        dropClient(s, st->addr);
                        goto next_socket;
                    }
                    if (st->rx.size() < 4 + sz)
                        break;

                    auto pkt = _packetProto->newPacket();
                    pkt->setAddress(st->addr);
                    pkt->setSize(sz);

                    std::memcpy(pkt->buffer(), st->rx.data() + 4, sz);
                    st->rx.erase(st->rx.begin(), st->rx.begin() + 4 + sz);

                    {
                        std::scoped_lock qlock(_queueMutex);
                        _queue.push(std::move(pkt));
                    }
                }
            }
        next_socket:;
        }
    }

    bool TCPServer::popPacket(std::shared_ptr<IPacket> &pkt)
    {
        std::scoped_lock lock(_queueMutex);
        if (_queue.empty())
            return false;
        pkt = std::move(_queue.front());
        _queue.pop();
        return true;
    }

    bool TCPServer::sendAll(const socketHandle clientFd, const uint8_t *data, const size_t len) const
    {
        size_t off = 0;
        while (off < len) {
            const auto r = _netWrapper->send(clientFd, data + off, len - off, 0);
            if (r <= 0)
                return false;
            off += static_cast<size_t>(r);
        }
        return true;
    }

    bool TCPServer::sendPacket(const IPacket &pkt)
    {
        const sockaddr_in *addr = pkt.address();
        if (!addr)
            return false;

        socketHandle dest = kInvalidSocket;
        {
            std::scoped_lock lock(_mutex);
            const auto it = _endpointToSock.find(keyFrom_(*addr));
            if (it == _endpointToSock.end())
                return false;
            dest = it->second;
        }

        const auto sz = static_cast<uint32_t>(pkt.size());
        if (sz == 0 || sz > MaxFrame)
            return false;

        std::vector<uint8_t> frame(4 + sz);
        writeU32BE(frame.data(), sz);
        std::memcpy(frame.data() + 4, pkt.buffer(), sz);

        return sendAll(dest, frame.data(), frame.size());
    }
} // namespace Net::Server
