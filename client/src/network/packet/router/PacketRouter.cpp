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
        const std::uint8_t *payload = raw;
        const std::size_t payloadSize = total;

        dispatchPacket(header, payload, payloadSize);
    }

    void PacketRouter::dispatchPacket(
        const HeaderData &header, const std::uint8_t *payload, const std::size_t payloadSize) const
    {
        switch (header.type) {
            case Net::Protocol::ACCEPT: handleAccept(); break;
            case Net::Protocol::REJECT: handleReject(); break;
            case Net::Protocol::GAME_OVER: handleGameOver(); break;
            case Net::Protocol::PONG: handlePong(); break;
            case Net::Protocol::ENTITY_CREATE: handleEntityCreate(payload, payloadSize); break;
            case Net::Protocol::ENTITY_DESTROY: handleEntityDestroy(payload, payloadSize); break;
            case Net::Protocol::SNAPSHOT: handleSnapEntity(payload, payloadSize); break;
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
        _sink->onAccept();
    }

    void PacketRouter::handleReject() const
    {
        _sink->onReject();
    }

    void PacketRouter::handlePong() const
    {
        _sink->onPong();
    }

    void PacketRouter::handleGameOver() const
    {
        _sink->onGameOver();
    }

    void PacketRouter::handleEntityCreate(const uint8_t *payload, const size_t size) const
    {
        if (size < sizeof(EntityCreateData)) {
            std::cerr << "{PacketRouter::handleEntityCreate} Dropped: payload too small\n";
            return;
        }

        EntityCreateData net{};
        std::memcpy(&net, payload, sizeof(EntityCreateData));

        EntityCreate evt{};
        evt.id = be64toh(net.id);
        evt.x = ntohf(net.x);
        evt.y = ntohf(net.y);
        evt.spriteId = ntohs(net.spriteId);
        _sink->onEntityCreate(evt);
    }

    void PacketRouter::handleEntityDestroy(const uint8_t *payload, const size_t size) const
    {
        EntityDestroyData data{};

        if (size < sizeof(EntityDestroyData)) {
            std::cerr << "{PacketRouter::handleEntityDestroy} Dropped: payload too small\n";
            return;
        }
        std::memcpy(&data, payload, sizeof(EntityDestroyData));
        EntityDestroy entityDestroy{};
        entityDestroy.id = be64toh(data.id);
        _sink->onEntityDestroy(entityDestroy);
    }

    void PacketRouter::handleSnapEntity(const uint8_t *payload, const size_t size) const
    {
        if (size < sizeof(SnapshotBatchHeader)) {
            std::cerr << "Snapshot batch too small\n";
            return;
        }

        SnapshotBatchHeader batch{};
        std::memcpy(&batch, payload, sizeof(batch));

        const uint16_t count = ntohs(batch.count);
        const uint8_t *cursor = payload + sizeof(SnapshotBatchHeader);

        for (uint16_t i = 0; i < count; ++i) {
            if (cursor + sizeof(SnapshotEntityData) > payload + size)
                break;

            SnapshotEntityData entityData{};
            std::memcpy(&entityData, cursor, sizeof(entityData));

            SnapshotEntity entity{};
            entity.id = be64toh(entityData.id);
            entity.x = ntohf(entityData.x);
            entity.y = ntohf(entityData.y);
            entity.spriteId = ntohs(entityData.spriteId);

            _sink->onSnapshot(entity);
            cursor += sizeof(SnapshotEntityData);
        }
    }
} // namespace Ecs
