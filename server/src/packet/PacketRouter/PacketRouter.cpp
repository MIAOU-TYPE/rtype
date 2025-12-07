/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PacketRouter
*/

#include "PacketRouter.hpp"

PacketRouter::PacketRouter(const std::shared_ptr<SessionManager> &sessions, const std::shared_ptr<IMessageSink> &sink)
    : _sessions(sessions), _sink(sink)
{
}

bool PacketRouter::validateHeader(const Net::IPacket &pkt, const HeaderData &header) const
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

    std::uint16_t declaredSize = ntohs(header.size);
    if (declaredSize != pkt.size()) {
        std::cerr << "{PacketRouter} Dropped: size mismatch "
                  << "(header=" << declaredSize << ", actual=" << pkt.size() << ")" << std::endl;
        return false;
    }

    return true;
}

bool PacketRouter::isPacketValid(const std::shared_ptr<Net::IPacket> &packet) const noexcept
{
    if (!packet)
        return false;

    if (packet->size() < sizeof(HeaderData)) {
        std::cerr << "{PacketRouter} Dropped: packet too small\n";
        return false;
    }
    return true;
}

bool PacketRouter::extractHeader(const Net::IPacket &packet, HeaderData &outHeader) const noexcept
{
    std::memcpy(&outHeader, packet.buffer(), sizeof(HeaderData));

    if (!validateHeader(packet, outHeader))
        return false;
    return true;
}

int PacketRouter::resolveSession(const Net::IPacket &packet)
{
    const sockaddr_in *addr = packet.address();
    if (!addr) {
        std::cerr << "{PacketRouter} Dropped: null address in packet\n";
        return -1;
    }
    return _sessions->getOrCreateSession(*addr);
}

void PacketRouter::dispatchPacket(
    int sessionId, const HeaderData &header, const uint8_t *payload, std::size_t payloadSize)
{
    switch (header.type) {
        case Net::Protocol::CONNECT: handleConnect(sessionId); break;
        case Net::Protocol::INPUT: handleInput(sessionId, payload, payloadSize); break;
        case Net::Protocol::PING: handlePing(sessionId, payload, payloadSize); break;
        case Net::Protocol::DISCONNECT: handleDisconnect(sessionId); break;
        default: std::cerr << "{PacketRouter} Unknown packet type: " << static_cast<int>(header.type) << '\n'; break;
    }
}

void PacketRouter::handlePacket(const std::shared_ptr<Net::IPacket> &packet)
{
    if (!isPacketValid(packet))
        return;

    HeaderData header{};
    if (!extractHeader(*packet, header))
        return;

    int sessionId = resolveSession(*packet);
    if (sessionId < 0)
        return;

    const std::uint8_t *raw = packet->buffer();
    const std::size_t total = packet->size();
    const std::size_t payloadSize = total - sizeof(HeaderData);
    const std::uint8_t *payload = raw + sizeof(HeaderData);

    dispatchPacket(sessionId, header, payload, payloadSize);
}

void PacketRouter::handleConnect(int sessionId)
{
    _sink->onPlayerConnect(sessionId);
}

void PacketRouter::handleInput(int sessionId, const std::uint8_t *payload, std::size_t payloadSize)
{
    if (!payload || payloadSize < 1) {
        std::cerr << "{PacketRouter::handleInput} Dropped INPUT: missing payload" << std::endl;
        return;
    }

    std::uint8_t flags = payload[0];

    PlayerInputMessage msg{};
    msg.up = (flags & 0x01u) != 0;
    msg.down = (flags & 0x02u) != 0;
    msg.left = (flags & 0x04u) != 0;
    msg.right = (flags & 0x08u) != 0;
    msg.shoot = (flags & 0x10u) != 0;

    _sink->onPlayerInput(sessionId, msg);
}

void PacketRouter::handlePing(int sessionId, const std::uint8_t *payload, std::size_t payloadSize)
{
    if (!payload || payloadSize < sizeof(std::uint64_t)) {
        std::cerr << "{PacketRouter::handlePing} Dropped PING: payload too small" << std::endl;
        return;
    }

    std::uint64_t timestampNet = 0;
    std::memcpy(&timestampNet, payload, sizeof(std::uint64_t));
    std::uint64_t timestamp = ntohll(timestampNet);

    _sink->onPing(sessionId, timestamp);
}

void PacketRouter::handleDisconnect(int sessionId)
{
    _sink->onPlayerDisconnect(sessionId);
    _sessions->removeSession(sessionId);
}
