/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** TCPClient
*/

#include "TCPClient.hpp"

#include <iostream>
#include "TCPPacket.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <errno.h>
    #include <unistd.h>
#endif

namespace Network
{
    bool TCPClient::wouldBlock() noexcept
    {
#ifdef _WIN32
        const int e = WSAGetLastError();
        return e == WSAEWOULDBLOCK;
#else
        return errno == EWOULDBLOCK || errno == EAGAIN;
#endif
    }

    TCPClient::TCPClient(std::shared_ptr<Net::NetWrapper> net)
        : _netWrapper(std::move(net)), _packetProto(std::make_shared<Net::TCPPacket>())
    {
        if (!_netWrapper)
            throw TCPClientError("{TCPClient} NetWrapper is null");
        if (!_packetProto)
            throw TCPClientError("{TCPClient} packet proto is null");
    }

    TCPClient::~TCPClient()
    {
        close();
    }

    void TCPClient::resetState() noexcept
    {
        _rx.clear();
        _rxPos = 0;

        std::scoped_lock lk(_txMutex);
        _tx.clear();
        _txPos = 0;

        {
            std::scoped_lock qlk(_queueMutex);
            while (!_queue.empty())
                _queue.pop();
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

            if (::connect(_socketFd, reinterpret_cast<const sockaddr *>(&_serverAddr), sizeof(_serverAddr)) != 0)
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
    }

    void TCPClient::setNonBlocking(bool nonBlocking)
    {
        _nonBlocking = nonBlocking;
        if (_socketFd == kInvalidSocket)
            return;
        (void) _netWrapper->setNonBlocking(_socketFd, _nonBlocking);
    }

    std::shared_ptr<Net::IPacket> TCPClient::getTemplatedPacket() const noexcept
    {
        return _packetProto;
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

        while (_txPos < _tx.size()) {
            const auto *ptr = reinterpret_cast<const char *>(_tx.data() + _txPos);
            const std::size_t remaining = _tx.size() - _txPos;

            const auto sent = _netWrapper->send(_socketFd, ptr, remaining, 0);
            if (sent > 0) {
                _txPos += static_cast<std::size_t>(sent);
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

        if (_txPos == _tx.size()) {
            _tx.clear();
            _txPos = 0;
        }
    }

    bool TCPClient::sendPacket(const Net::IPacket &pkt)
    {
        if (_socketFd == kInvalidSocket || !_isRunning.load())
            return false;

        const auto size = static_cast<std::uint32_t>(pkt.size());
        if (size == 0 || size > MAX_FRAME)
            return false;

        const std::uint32_t beSize = htonl(size);

        std::scoped_lock lk(_txMutex);

        const std::size_t old = _tx.size();
        _tx.resize(old + 4 + size);

        std::memcpy(_tx.data() + old, &beSize, 4);
        std::memcpy(_tx.data() + old + 4, pkt.buffer(), size);

        lk.~scoped_lock();
        flushWrites();
        return true;
    }

    void TCPClient::parseFrames() noexcept
    {
        if (_rxPos > 0 && _rxPos > _rx.size() / 2) {
            _rx.erase(_rx.begin(), _rx.begin() + static_cast<std::ptrdiff_t>(_rxPos));
            _rxPos = 0;
        }

        while ((_rx.size() - _rxPos) >= 4) {
            std::uint32_t beSize = 0;
            std::memcpy(&beSize, _rx.data() + _rxPos, 4);

            const std::uint32_t size = ntohl(beSize);
            if (size == 0 || size > MAX_FRAME) {
                close();
                return;
            }

            if ((_rx.size() - _rxPos) < (4u + size))
                return;

            const std::uint8_t *payload = _rx.data() + _rxPos + 4;

            auto p = _packetProto->newPacket();
            if (!p || size > p->capacity()) {
                close();
                return;
            }

            p->setAddress(_serverAddr);
            p->setSize(size);
            std::memcpy(p->buffer(), payload, size);

            {
                std::scoped_lock qlk(_queueMutex);
                _queue.push(std::move(p));
            }

            _rxPos += (4u + size);
        }
    }

    void TCPClient::receivePackets()
    {
        if (_socketFd == kInvalidSocket || !_isRunning.load())
            return;

        // flush TX opportuniste
        flushWrites();

        std::array<std::uint8_t, 4096> tmp{};

        while (true) {
            const auto r = _netWrapper->recv(_socketFd, tmp.data(), tmp.size(), 0);

            if (r > 0) {
                const auto old = _rx.size();
                _rx.resize(old + static_cast<std::size_t>(r));
                std::memcpy(_rx.data() + old, tmp.data(), static_cast<std::size_t>(r));
                parseFrames();
                std::cout << "TCPClient: Received " << r << " bytes." << std::endl;

                if (!_nonBlocking)
                    break;
                continue;
            }

            if (r == 0) {
                close();
                return;
            }

            if (wouldBlock())
                break;

            close();
            return;
        }
    }
} // namespace Network
