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
            case Net::Protocol::SNAPSHOT: handleSnapEntity(payload, payloadSize); break;
            case Net::Protocol::SCORE: handleScore(payload, payloadSize); break;
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

    void PacketRouter::handleSnapEntity(const uint8_t *payload, const size_t size) const
    {
        if (size < sizeof(SnapshotBatchHeader)) {
            std::cerr << "{PacketRouter::handleSnapEntity} Snapshot batch too small\n";
            return;
        }

        SnapshotBatchHeader batch{};
        std::memcpy(&batch, payload, sizeof(batch));

        const uint16_t count = ntohs(batch.count);
        const uint8_t *cursor = payload + sizeof(SnapshotBatchHeader);

        std::vector<SnapshotEntity> entities;
        entities.reserve(count);

        for (uint16_t i = 0; i < count; ++i) {
            if (cursor + sizeof(SnapshotEntityData) > payload + size)
                break;

            SnapshotEntityData entityData{};
            std::memcpy(&entityData, cursor, sizeof(entityData));

            SnapshotEntity entity{};
            entity.id = be64toh(entityData.id);
            entity.x = ntohf(entityData.x);
            entity.y = ntohf(entityData.y);
            entity.spriteId = ntohl(entityData.spriteId);

            entities.push_back(entity);
            cursor += sizeof(SnapshotEntityData);
        }
        _sink->onSnapshot(entities);
    }

    void PacketRouter::handleScore(const uint8_t *payload, size_t size) const
    {
        if (size < sizeof(ScoreData)) {
            std::cerr << "{PacketRouter::handleScore} Score packet too small\n";
            return;
        }

        ScoreData scoreData{};
        std::memcpy(&scoreData, payload, sizeof(scoreData));
        const uint32_t score = ntohl(scoreData.score);

        _sink->onScore(score);
    }
} // namespace Ecs
