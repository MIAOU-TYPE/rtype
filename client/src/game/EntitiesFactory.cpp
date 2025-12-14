/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntitiesFactory
*/

#include "EntitiesFactory.hpp"

namespace Ecs
{
    void EntitiesFactory::handlePacket(const std::shared_ptr<Net::IPacket> &packet) const
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

    void EntitiesFactory::dispatchPacket(
        const HeaderData &header, const std::uint8_t *payload, const std::size_t payloadSize) const
    {
        (void) payload;
        (void) payloadSize;

        switch (header.type) {
            case Net::Protocol::ACCEPT: handleAccept(); break;
            case Net::Protocol::GAME_OVER: handleGameOver(); break;
            case Net::Protocol::PONG: handlePong(); break;
            case Net::Protocol::REJECT: handleReject(); break;
            case Net::Protocol::ENTITY_CREATE: handleEntityCreate(payload, payloadSize); break;
            case Net::Protocol::ENTITY_DESTROY: handleEntityDestroy(payload, payloadSize); break;
            case Net::Protocol::SNAPSHOT: handleSnapEntity(payload, payloadSize); break;
            default:
                std::cerr << "{EntitiesFactory::dispatchPacket} Unknown packet type: " << static_cast<int>(header.type)
                          << '\n';
                break;
        }
    }

    bool EntitiesFactory::isHeaderValid(const Net::IPacket &packet, const HeaderData &header)
    {
        if (packet.size() < sizeof(HeaderData)) {
            std::cerr << "{EntitiesFactory::isHeaderValid} Dropped: packet too small\n";
            return false;
        }

        if (header.version != 1) {
            std::cerr << "{EntitiesFactory::isHeaderValid} Dropped: wrong protocol version "
                      << static_cast<int>(header.version) << " (expected " << static_cast<int>(1) << ")" << std::endl;
            return false;
        }

        if (const std::uint16_t declaredSize = ntohs(header.size);
            declaredSize != static_cast<std::uint16_t>(packet.size())) {
            std::cerr << "{EntitiesFactory::isHeaderValid} Dropped: size mismatch (header="
                    + std::to_string(declaredSize)
                    + ", actual=" + std::to_string(static_cast<std::uint16_t>(packet.size())) + ")\n";
            return false;
        }

        return true;
    }

    bool EntitiesFactory::isPacketValid(const std::shared_ptr<Net::IPacket> &packet) const noexcept
    {
        if (!packet)
            return false;

        if (packet->size() < sizeof(HeaderData)) {
            std::cerr << "{EntitiesFactory::isPacketValid} Dropped: packet too small\n";
            return false;
        }
        return true;
    }

    bool EntitiesFactory::extractHeader(const Net::IPacket &packet, HeaderData &outHeader) const noexcept
    {
        std::memcpy(&outHeader, packet.buffer(), sizeof(HeaderData));

        if (!isHeaderValid(packet, outHeader))
            return false;
        return true;
    }

    void EntitiesFactory::handleAccept() const
    {
        // Implementation for handling ACCEPT packet
    }

    void EntitiesFactory::handleReject() const
    {
        // Implementation for handling REJECT packet
    }

    void EntitiesFactory::handlePong() const
    {
        // Implementation for handling PONG packet
    }

    void EntitiesFactory::handleGameOver() const
    {
        // Implementation for handling GAME_OVER packet
    }

    void EntitiesFactory::handleEntityCreate(const std::uint8_t *payload, const std::size_t payloadSize) const
    {
        // Implementation for handling ENTITY_CREATE packet
    }

    void EntitiesFactory::handleEntityDestroy(const std::uint8_t *payload, const std::size_t payloadSize) const
    {
        // Implementation for handling ENTITY_DESTROY packet
    }

    void EntitiesFactory::handleSnapEntity(const std::uint8_t *payload, const std::size_t payloadSize) const
    {
        // Implementation for handling SNAPSHOT packet
    }
} // namespace Ecs
