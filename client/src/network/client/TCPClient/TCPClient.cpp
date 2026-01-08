/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** TCPClient
*/

#include "TCPClient.hpp"

namespace Network
{
    namespace
    {
        bool wouldBlock() noexcept
        {
#ifdef _WIN32
            const int e = WSAGetLastError();
            return e == WSAEWOULDBLOCK;
#else
            return errno == EWOULDBLOCK || errno == EAGAIN;
#endif
        }
    } // namespace

    TCPClient::TCPClient(std::shared_ptr<Net::NetWrapper> net)
        : _netWrapper(std::move(net)), _rx(RX_CAPACITY), _tx(TX_CAPACITY)
    {
        if (!_netWrapper)
            throw TCPClientError("{TCPClient} NetWrapper is null");
    }

    TCPClient::~TCPClient()
    {
        close();
    }

    void TCPClient::resetState() noexcept
    {
        _rx.clear();

        {
            std::scoped_lock lk(_txMutex);
            _tx.clear();
        }

        {
            std::scoped_lock qlk(_queueMutex);
            _queue = {};
        }
    }

    void TCPClient::start()
    {
        try {
            if (!ANetClient::isStoredIpCorrect() || !ANetClient::isStoredPortCorrect())
                throw TCPClientError("{TCPClient::start} invalid IP/Port");

            if (_netWrapper->initNetwork() != 0)
                throw TCPClientError("{TCPClient::start} initNetwork failed");

            if (_socketFd = _netWrapper->socket(AF_INET, SOCK_STREAM, 0); _socketFd == kInvalidSocket)
                throw TCPClientError("{TCPClient::start} socket() failed");

            std::memset(&_serverAddr, 0, sizeof(_serverAddr));
            _serverAddr.sin_family = AF_INET;
            _serverAddr.sin_port = htons(static_cast<std::uint16_t>(_port));
            if (::inet_pton(AF_INET, _ip.c_str(), &_serverAddr.sin_addr) != 1)
                throw TCPClientError("{TCPClient::start} inet_pton failed");

            if (_netWrapper->connect(_socketFd, reinterpret_cast<const sockaddr *>(&_serverAddr), sizeof(_serverAddr))
                != 0)
                throw TCPClientError("{TCPClient::start} connect failed");

            setNonBlocking(true);
            ANetClient::setRunning(true);
        } catch (...) {
            close();
            throw;
        }
    }

    void TCPClient::close()
    {
        if (!_isRunning.exchange(false) && _socketFd == kInvalidSocket)
            return;

        resetState();
        if (_socketFd != kInvalidSocket) {
            _netWrapper->closeSocket(_socketFd);
            _socketFd = kInvalidSocket;
        }
        (void) _netWrapper->cleanupNetwork();
        std::cout << "{TCPClient::close} TCP Client stopped.\n";
    }

    void TCPClient::setNonBlocking(const bool nonBlocking)
    {
        _nonBlocking = nonBlocking;
        if (_socketFd == kInvalidSocket)
            return;
        (void) _netWrapper->setNonBlocking(_socketFd, _nonBlocking);
    }

    std::shared_ptr<Net::IPacket> TCPClient::getTemplatedPacket() const noexcept
    {
        return std::make_shared<Net::TCPPacket>();
    }

    bool TCPClient::popPacket(std::shared_ptr<Net::IPacket> &pkt)
    {
        std::scoped_lock lock(_queueMutex);
        if (_queue.empty())
            return false;
        pkt = std::move(_queue.front());
        _queue.pop();
        return true;
    }

    void TCPClient::flushWrites() noexcept
    {
        std::scoped_lock lk(_txMutex);

        std::array<std::uint8_t, 4096> tmp{};

        while (_tx.readable() > 0) {
            const std::size_t want = std::min<std::size_t>(_tx.readable(), tmp.size());

            if (!_tx.peek(tmp.data(), want)) {
                close();
                return;
            }

            const auto sent = _netWrapper->send(_socketFd, reinterpret_cast<const char *>(tmp.data()), want, 0);

            if (sent > 0) {
                const auto n = static_cast<std::size_t>(sent);
                (void) _tx.read(tmp.data(), n);
                continue;
            }

            if (sent == 0) {
                close();
                return;
            }
            if (wouldBlock())
                break;

            close();
            return;
        }
    }

    bool TCPClient::sendPacket(const Net::IPacket &pkt)
    {
        std::uint32_t size = 0;
        if (!isSendable(pkt, size))
            return false;

        const std::uint32_t beSize = htonl(size);

        if (tryEnqueueFrame(beSize, pkt, size)) {
            flushWrites();
            return true;
        }
        flushWrites();

        if (!tryEnqueueFrame(beSize, pkt, size))
            return false;

        flushWrites();
        return true;
    }

    bool TCPClient::isSendable(const Net::IPacket &pkt, std::uint32_t &outSize) const noexcept
    {
        if (_socketFd == kInvalidSocket || !_isRunning.load())
            return false;

        outSize = static_cast<std::uint32_t>(pkt.size());
        if (outSize == 0 || outSize > MAX_FRAME)
            return false;
        return true;
    }

    bool TCPClient::tryEnqueueFrame(const std::uint32_t beSize, const Net::IPacket &pkt, const std::uint32_t size)
    {
        std::scoped_lock lk(_txMutex);
        return enqueueFrameLocked(beSize, pkt, size);
    }

    bool TCPClient::enqueueFrameLocked(const std::uint32_t beSize, const Net::IPacket &pkt, const std::uint32_t size)
    {
        if (_tx.writable() < (4u + size))
            return false;

        if (!_tx.write(reinterpret_cast<const std::uint8_t *>(&beSize), 4))
            return false;

        if (!_tx.write(pkt.buffer(), size))
            return false;

        return true;
    }

    void TCPClient::parseFrames() noexcept
    {
        while (_rx.readable() >= 4u) {
            std::uint8_t hdr[4]{};
            if (!_rx.peek(hdr, 4))
                return close();

            std::uint32_t beSize = 0;
            std::memcpy(&beSize, hdr, 4);

            const std::uint32_t size = ntohl(beSize);
            if (size == 0 || size > MAX_FRAME)
                return close();

            if (_rx.readable() < (4u + size))
                return;

            std::uint8_t throwAway[4]{};
            (void) _rx.read(throwAway, 4);

            auto p = getTemplatedPacket();
            if (!p || size > p->capacity())
                return close();

            p->setAddress(_serverAddr);
            p->setSize(size);

            if (!_rx.read(p->buffer(), size))
                return close();

            {
                std::scoped_lock qlk(_queueMutex);
                _queue.push(std::move(p));
            }
        }
    }

    void TCPClient::receivePackets()
    {
        if (_socketFd == kInvalidSocket || !_isRunning.load())
            return;

        flushWrites();
        std::array<std::uint8_t, 4096> tmp{};

        while (true) {
            const auto r = _netWrapper->recv(_socketFd, tmp.data(), tmp.size(), 0);

            if (r > 0) {
                if (const auto n = static_cast<std::size_t>(r); !_rx.write(tmp.data(), n))
                    return close();
                parseFrames();
                if (!_nonBlocking)
                    break;
                continue;
            }
            if (r == 0)
                return close();
            if (wouldBlock())
                break;
            return close();
        }
    }
} // namespace Network
