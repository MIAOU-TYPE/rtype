/*
** EPITECH PROJECT, 2025
** RType
** File description:
** UDPPacketRouter
*/

#include "UDPPacketRouter.hpp"
using namespace Net;

UDPPacketRouter::UDPPacketRouter(
    const std::shared_ptr<Server::ISessionManager> &sessions, const std::shared_ptr<Engine::RoomManager> &roomManager)
    : _sessions(sessions), _roomManager(roomManager)
{
}

bool UDPPacketRouter::validateHeader(const IPacket &pkt, const HeaderData &header)
{
    if (pkt.size() < sizeof(HeaderData)) {
        std::cerr << "{UDPPacketRouter::validateHeader} Dropped: packet too small" << std::endl;
        return false;
    }

    if (std::memcmp(header.magic, kPacketMagic, 4) != 0) {
        std::cerr << "{UDPPacketRouter::validateHeader} Dropped: bad magic\n";
        return false;
    }

    if (header.version != PROTOCOL_VERSION) {
        std::cerr << "{UDPPacketRouter} Dropped: wrong protocol version " << static_cast<int>(header.version)
                  << " (expected " << static_cast<int>(PROTOCOL_VERSION) << ")" << std::endl;
        return false;
    }

    if (const std::uint16_t declaredSize = ntohs(header.size); declaredSize != pkt.size()) {
        std::cerr << "{UDPPacketRouter} Dropped: size mismatch "
                  << "(header=" << declaredSize << ", actual=" << pkt.size() << ")" << std::endl;
        return false;
    }

    return true;
}

bool UDPPacketRouter::isPacketValid(const std::shared_ptr<IPacket> &packet) noexcept
{
    if (!packet)
        return false;

    if (packet->size() < sizeof(HeaderData)) {
        std::cerr << "{UDPPacketRouter} Dropped: packet too small\n";
        return false;
    }
    return true;
}

bool UDPPacketRouter::extractHeader(const IPacket &packet, HeaderData &outHeader) noexcept
{
    if (!packet.buffer())
        return false;
    if (packet.size() < sizeof(HeaderData))
        return false;

    std::memcpy(&outHeader, packet.buffer(), sizeof(HeaderData));
    return validateHeader(packet, outHeader);
}

void UDPPacketRouter::dispatchPacket(
    const int sessionId, const HeaderData &header, const uint8_t *payload, const std::size_t payloadSize) const
{
    switch (header.type) {
        case Protocol::UDP::INPUT: handleInput(sessionId, payload, payloadSize); break;

        case Protocol::UDP::PING:
            if (payloadSize != sizeof(DefaultData)) {
                std::cerr << "{UDPPacketRouter} Dropped PING: bad size" << std::endl;
                break;
            }
            handlePing(sessionId);
            break;

        case Protocol::UDP::DISCONNECT:
            if (payloadSize != sizeof(DefaultData)) {
                std::cerr << "{UDPPacketRouter} Dropped DISCONNECT: bad size\n";
                break;
            }
            handleDisconnect(sessionId);
            break;
        default: std::cerr << "{UDPPacketRouter} Unknown packet type: " << static_cast<int>(header.type) << '\n'; break;
    }
}

void UDPPacketRouter::handlePacket(const std::shared_ptr<IPacket> &packet) const
{
    if (!isPacketValid(packet))
        return;

    const sockaddr_in *from = packet->address();
    if (!from) {
        std::cerr << "{UDPPacketRouter} Dropped: null address in packet\n";
        return;
    }

    HeaderData header{};
    if (!extractHeader(*packet, header))
        return;

    const std::uint8_t *raw = packet->buffer();
    const std::size_t total = packet->size();

    if (header.type == Protocol::UDP::CONNECT) {
        if (total != sizeof(ConnectData)) {
            std::cerr << "{UDPPacketRouter} Dropped CONNECT: bad size\n";
            return;
        }

        if (!raw)
            return;
        ConnectData cd{};
        std::memcpy(&cd, raw, sizeof(cd));

        const uint32_t sid = ntohl(cd.sessionId);
        const uint64_t token =
            (static_cast<uint64_t>(ntohl(cd.tokenHi)) << 32) | static_cast<std::uint64_t>(ntohl(cd.tokenLo));

        if (_sessions->getUdpToken(static_cast<int>(sid)) != token) {
            std::cerr << "{UDPPacketRouter} Dropped CONNECT: bad token\n";
            return;
        }

        if (!_sessions->bindUdp(static_cast<int>(sid), *from))
            std::cerr << "{UDPPacketRouter} CONNECT: bindUdp failed (unknown session?)\n";
        return;
    }
    const int sessionId = _sessions->getSessionIdFromUdp(*from);
    if (sessionId < 0)
        return;
    dispatchPacket(sessionId, header, raw, total);
}

void UDPPacketRouter::handleInput(const int sessionId, const std::uint8_t *payload, const std::size_t payloadSize) const
{
    if (!payload || payloadSize != sizeof(PlayerInputData)) {
        std::cerr << "{UDPPacketRouter::handleInput} Dropped INPUT: bad size\n";
        return;
    }

    PlayerInputData pkt{};
    std::memcpy(&pkt, payload, sizeof(pkt));

    const std::uint8_t flags = pkt.flags;
    const bool up = (flags & 0x01u) != 0;
    const bool down = (flags & 0x02u) != 0;
    const bool left = (flags & 0x04u) != 0;
    const bool right = (flags & 0x08u) != 0;
    const bool shoot = (flags & 0x10u) != 0;

    _roomManager->onPlayerInput(sessionId, Game::InputComponent{up, down, left, right, shoot});
}

void UDPPacketRouter::handlePing(const int sessionId) const
{
    _roomManager->onPing(sessionId);
}

void UDPPacketRouter::handleDisconnect(const int sessionId) const
{
    _roomManager->onPlayerDisconnect(sessionId);
}
