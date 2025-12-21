/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PacketRouter
*/

#include "PacketRouter.hpp"
using namespace Net;

PacketRouter::PacketRouter(
    const std::shared_ptr<Server::ISessionManager> &sessions, const std::shared_ptr<IMessageSink> &sink)
    : _sessions(sessions), _sink(sink)
{
}

bool PacketRouter::validateHeader(const IPacket &pkt, const HeaderData &header)
{
    if (pkt.size() < sizeof(HeaderData)) {
        std::cerr << "{PacketRouter::validateHeader} Dropped: packet too small)" << std::endl;
        return false;
    }

    if (header.version != PROTOCOL_VERSION) {
        std::cerr << "{PacketRouter} Dropped: wrong protocol version " << static_cast<int>(header.version)
                  << " (expected " << static_cast<int>(PROTOCOL_VERSION) << ")" << std::endl;
        return false;
    }

    if (const std::uint16_t declaredSize = ntohs(header.size); declaredSize != pkt.size()) {
        std::cerr << "{PacketRouter} Dropped: size mismatch "
                  << "(header=" << declaredSize << ", actual=" << pkt.size() << ")" << std::endl;
        return false;
    }

    return true;
}

bool PacketRouter::isPacketValid(const std::shared_ptr<IPacket> &packet) noexcept
{
    if (!packet)
        return false;

    if (packet->size() < sizeof(HeaderData)) {
        std::cerr << "{PacketRouter} Dropped: packet too small\n";
        return false;
    }
    return true;
}

bool PacketRouter::extractHeader(const IPacket &packet, HeaderData &outHeader) noexcept
{
    std::memcpy(&outHeader, packet.buffer(), sizeof(HeaderData));

    if (!validateHeader(packet, outHeader))
        return false;
    return true;
}

int PacketRouter::resolveSession(const IPacket &packet) const
{
    const sockaddr_in *addr = packet.address();
    if (!addr) {
        std::cerr << "{PacketRouter} Dropped: null address in packet\n";
        return -1;
    }
    return _sessions->getOrCreateSession(*addr);
}

void PacketRouter::dispatchPacket(
    const int sessionId, const HeaderData &header, const uint8_t *payload, std::size_t payloadSize) const
{
    switch (header.type) {
        case Protocol::CONNECT: handleConnect(sessionId); break;
        case Protocol::INPUT: handleInput(sessionId, payload, payloadSize); break;
        case Protocol::PING: handlePing(sessionId); break;
        case Protocol::DISCONNECT: handleDisconnect(sessionId); break;
        default: std::cerr << "{PacketRouter} Unknown packet type: " << static_cast<int>(header.type) << '\n'; break;
    }
}

void PacketRouter::handlePacket(const std::shared_ptr<IPacket> &packet) const
{
    if (!isPacketValid(packet))
        return;

    HeaderData header{};
    if (!extractHeader(*packet, header))
        return;

    const int sessionId = resolveSession(*packet);
    if (sessionId < 0)
        return;

    const std::uint8_t *raw = packet->buffer();
    const std::size_t total = packet->size();
    const std::size_t payloadSize = total - sizeof(HeaderData);
    const std::uint8_t *payload = raw + sizeof(HeaderData);

    dispatchPacket(sessionId, header, payload, payloadSize);
}

void PacketRouter::handleConnect(const int sessionId) const
{
    _sink->onPlayerConnect(sessionId);
}

void PacketRouter::handleInput(const int sessionId, const std::uint8_t *payload, const std::size_t payloadSize) const
{
    if (!payload || payloadSize < sizeof(uint8_t)) {
        std::cerr << "{PacketRouter::handleInput} Dropped INPUT: missing payload" << std::endl;
        return;
    }

    const std::uint8_t flags = payload[0];
    const bool up = (flags & 0x01u) != 0;
    const bool down = (flags & 0x02u) != 0;
    const bool left = (flags & 0x04u) != 0;
    const bool right = (flags & 0x08u) != 0;
    const bool shoot = (flags & 0x10u) != 0;

    _sink->onPlayerInput(sessionId, Game::InputComponent{up, down, left, right, shoot});
}

void PacketRouter::handlePing(const int sessionId) const
{
    _sink->onPing(sessionId);
}

void PacketRouter::handleDisconnect(const int sessionId) const
{
    _sink->onPlayerDisconnect(sessionId);
    _sessions->removeSession(sessionId);
}
