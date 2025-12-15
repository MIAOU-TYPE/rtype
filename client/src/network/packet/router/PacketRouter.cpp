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
        switch (header.type) {
            case Net::Protocol::ACCEPT: handleAccept(); break;
            case Net::Protocol::GAME_OVER: handleGameOver(); break;
            case Net::Protocol::PONG: handlePong(); break;
            case Net::Protocol::REJECT: handleReject(); break;
            case Net::Protocol::ENTITY_CREATE: handleEntityCreate(payload, payloadSize); break;
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

        if (header.version != PROTOCOL_VERSION) {
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
        _sink->onAccept();
    }

    void PacketRouter::handleReject() const
    {
        _sink->onReject();
        // Implementation for handling REJECT packet
    }

    void PacketRouter::handlePong() const
    {
        _sink->onPong();
        // Implementation for handling PONG packet
    }

    void PacketRouter::handleGameOver() const
    {
        _sink->onGameOver();
        // Implementation for handling GAME_OVER packet
    }

    void PacketRouter::handleEntityCreate(const uint8_t *payload, size_t size) const
    {
        EntityCreateData data{};
        std::cout << "Handling ENTITY_CREATE packet of size " << size << std::endl;
        std::cout << "EntityCreateData - id: " << data.id << ", type: " << static_cast<int>(data.header.type)
                  << ", x: " << data.x << ", y: " << data.y << "sprite:" << data.sprite
                  << "header.size: " << data.header.size << std::endl;

        if (size < sizeof(EntityCreateData)) {
            std::cerr << "{PacketRouter::handleEntityCreate} Dropped: payload too small\n";
            return;
        }
        std::memcpy(&data, payload, sizeof(EntityCreateData));
        _sink->onEntityCreate(data);
    }

    void PacketRouter::handleEntityDestroy() const
    {
        // Implementation for handling ENTITY_DESTROY packet
        EntityDestroyData data{};
        _sink->onEntityDestroy(data);
    }

    void PacketRouter::handleSnapEntity() const
    {
        // Implementation for handling SNAPSHOT packet
        _sink->onSnapshot();
    }
} // namespace Ecs
