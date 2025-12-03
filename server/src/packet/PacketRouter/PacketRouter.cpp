/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PacketRouter
*/

#include "PacketRouter.hpp"

PacketRouter::PacketRouter(std::shared_ptr<SessionManager> &sessions, std::shared_ptr<IMessageSink> &sink)
    : _sessions(sessions), _sink(sink)
{
}

bool PacketRouter::validateHeader(const Net::IServerPacket &pkt, const HeaderPacket &header) const
{
    if (pkt.size() < sizeof(HeaderPacket)) {
        std::cerr << "{PacketRouter::validateHeader} Dropped: packet too small (" << pkt.size() << " bytes)"
                  << std::endl;
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

void PacketRouter::handlePacket(const std::shared_ptr<Net::IServerPacket> &packet)
{
    if (!packet)
        return;

    if (packet->size() < sizeof(HeaderPacket)) {
        std::cerr << "{PacketRouter} Dropped: packet too small" << std::endl;
        return;
    }

    const std::uint8_t *rawBuffer = packet->buffer();

    HeaderPacket header{};
    std::memcpy(&header, rawBuffer, sizeof(HeaderPacket));

    if (!validateHeader(*packet, header))
        return;

    const sockaddr_in *addr = packet->address();
    if (!addr) {
        std::cerr << "{PacketRouter} Dropped: null address in packet" << std::endl;
        return;
    }

    int sessionId = _sessions->getOrCreateSession(*addr);

    const std::size_t totalSize = packet->size();
    const std::size_t payloadSize = totalSize - sizeof(HeaderPacket);
    const std::uint8_t *payload = rawBuffer + sizeof(HeaderPacket);

    switch (header.type) {
        case Net::Factory::CONNECT: handleConnect(sessionId); break;
        case Net::Factory::INPUT: handleInput(sessionId, payload, payloadSize); break;
        case Net::Factory::PING: handlePing(sessionId, payload, payloadSize); break;
        case Net::Factory::DISCONNECT: handleDisconnect(sessionId); break;
        default:
            std::cerr << "{PacketRouter} Unknown packet type: " << static_cast<int>(header.type) << std::endl;
            break;
    }
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
