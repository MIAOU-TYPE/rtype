/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketRouter
*/

#include "PacketRouter.hpp"

namespace Ecs
{
    PacketRouter::PacketRouter(const std::shared_ptr<IClientMessageSink> &sink) : _sink(sink)
    {
    }

    void PacketRouter::handlePacket(const std::shared_ptr<Net::IPacket> &packet) const
    {
        if (!isPacketValid(packet))
            return;

        HeaderData header{};
        if (!extractHeader(*packet, header))
            return;

        const std::uint8_t *raw = packet->buffer();
        const std::size_t total = packet->size();
        const std::size_t payloadSize = total - sizeof(HeaderData);
        const std::uint8_t *payload = raw + sizeof(HeaderData);

        dispatchPacket(header, payload, payloadSize);
    }

    void PacketRouter::dispatchPacket(
        const HeaderData &header, const std::uint8_t *payload, const std::size_t payloadSize) const
    {
        (void) payload;
        (void) payloadSize;

        switch (header.type) {
            case Net::Protocol::ACCEPT: handleAccept(); break;
            case Net::Protocol::GAME_OVER: handleGameOver(); break;
            case Net::Protocol::PONG: handlePong(); break;
            case Net::Protocol::REJECT: handleReject(); break;
            case Net::Protocol::ENTITY_CREATE: handleEntityCreate(); break;
            case Net::Protocol::ENTITY_DESTROY: handleEntityDestroy(); break;
            case Net::Protocol::SNAPSHOT: handleSnapEntity(); break;
            default:
                std::cerr << "{PacketRouter::dispatchPacket} Unknown packet type: " << static_cast<int>(header.type)
                          << '\n';
                break;
        }
    }

    bool PacketRouter::isHeaderValid(const Net::IPacket &packet, const HeaderData &header)
    {
        if (packet.size() < sizeof(HeaderData)) {
            std::cerr << "{PacketRouter::isHeaderValid} Dropped: packet too small\n";
            return false;
        }

        if (header.version != 1) {
            std::cerr << "{PacketRouter::isHeaderValid} Dropped: wrong protocol version "
                      << static_cast<int>(header.version) << " (expected " << static_cast<int>(1) << ")" << std::endl;
            return false;
        }

        if (const std::uint16_t declaredSize = ntohs(header.size);
            declaredSize != static_cast<std::uint16_t>(packet.size())) {
            std::cerr << "{PacketRouter::isHeaderValid} Dropped: size mismatch (header=" + std::to_string(declaredSize)
                    + ", actual=" + std::to_string(static_cast<std::uint16_t>(packet.size())) + ")\n";
            return false;
        }

        return true;
    }

    bool PacketRouter::isPacketValid(const std::shared_ptr<Net::IPacket> &packet) noexcept
    {
        if (!packet)
            return false;

        if (packet->size() < sizeof(HeaderData)) {
            std::cerr << "{PacketRouter::isPacketValid} Dropped: packet too small\n";
            return false;
        }
        return true;
    }

    bool PacketRouter::extractHeader(const Net::IPacket &packet, HeaderData &outHeader) noexcept
    {
        std::memcpy(&outHeader, packet.buffer(), sizeof(HeaderData));

        if (!isHeaderValid(packet, outHeader))
            return false;
        return true;
    }

    void PacketRouter::handleAccept() const
    {
        // Implementation for handling ACCEPT packet
    }

    void PacketRouter::handleReject() const
    {
        // Implementation for handling REJECT packet
    }

    void PacketRouter::handlePong() const
    {
        // Implementation for handling PONG packet
    }

    void PacketRouter::handleGameOver() const
    {
        // Implementation for handling GAME_OVER packet
    }

    void PacketRouter::handleEntityCreate() const
    {
        // Implementation for handling ENTITY_CREATE packet
    }

    void PacketRouter::handleEntityDestroy() const
    {
        // Implementation for handling ENTITY_DESTROY packet
    }

    void PacketRouter::handleSnapEntity() const
    {
        // Implementation for handling SNAPSHOT packet
    }
} // namespace Ecs
