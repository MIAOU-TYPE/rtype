/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPPacketRouter
*/

#include "UDPPacketRouter.hpp"

namespace Ecs
{
    UDPPacketRouter::UDPPacketRouter(const std::shared_ptr<IClientMessageSink> &sink) : _sink(sink)
    {
    }

    void UDPPacketRouter::handlePacket(const std::shared_ptr<Net::IPacket> &packet) const
    {
        if (!isPacketValid(packet))
            return;

        HeaderData header{};
        if (!extractHeader(*packet, header))
            return;

        const std::uint8_t *raw = packet->buffer();
        const std::size_t total = packet->size();

        dispatchPacket(header, raw, total);
    }

    void UDPPacketRouter::dispatchPacket(
        const HeaderData &header, const std::uint8_t *payload, const std::size_t payloadSize) const
    {
        switch (header.type) {
            case Net::Protocol::UDP::ACCEPT:
                if (payloadSize != sizeof(DefaultData))
                    break;
                handleAccept();
                break;
            case Net::Protocol::UDP::REJECT:
                if (payloadSize != sizeof(DefaultData))
                    break;
                handleReject();
                break;
            case Net::Protocol::UDP::GAME_OVER:
                if (payloadSize != sizeof(DefaultData))
                    break;
                handleGameOver();
                break;
            case Net::Protocol::UDP::PONG:
                if (payloadSize != sizeof(DefaultData))
                    break;
                handlePong();
                break;
            case Net::Protocol::UDP::SNAPSHOT: handleSnapEntity(payload, payloadSize); break;
            case Net::Protocol::UDP::SCORE: handleScore(payload, payloadSize); break;
            default:
                std::cerr << "{UDPPacketRouter::dispatchPacket} Unknown packet type: " << static_cast<int>(header.type)
                          << '\n';
                break;
        }
    }

    bool UDPPacketRouter::isHeaderValid(const Net::IPacket &packet, const HeaderData &header)
    {
        if (packet.size() < sizeof(HeaderData)) {
            std::cerr << "{UDPPacketRouter::isHeaderValid} Dropped: packet too small\n";
            return false;
        }

        if (std::memcmp(header.magic, kPacketMagic, 4) != 0) {
            std::cerr << "{UDPPacketRouter::isHeaderValid} Dropped: bad magic\n";
            return false;
        }

        if (header.version != PROTOCOL_VERSION) {
            std::cerr << "{UDPPacketRouter::isHeaderValid} Dropped: wrong protocol version "
                      << static_cast<int>(header.version) << " (expected " << static_cast<int>(PROTOCOL_VERSION)
                      << ")\n";
            return false;
        }
        if (const std::uint16_t declaredSize = header.size; declaredSize != static_cast<std::uint16_t>(packet.size())) {
            std::cerr << "{UDPPacketRouter::isHeaderValid} Dropped: size mismatch (header=" << declaredSize
                      << ", actual=" << packet.size() << ")\n";
            return false;
        }
        return true;
    }

    bool UDPPacketRouter::isPacketValid(const std::shared_ptr<Net::IPacket> &packet) noexcept
    {
        if (!packet)
            return false;

        if (packet->size() < sizeof(HeaderData)) {
            std::cerr << "{UDPPacketRouter::isPacketValid} Dropped: packet too small\n";
            return false;
        }
        return true;
    }

    bool UDPPacketRouter::extractHeader(const Net::IPacket &packet, HeaderData &outHeader) noexcept
    {
        if (!packet.buffer())
            return false;
        if (packet.size() < sizeof(HeaderData))
            return false;

        std::memcpy(&outHeader, packet.buffer(), sizeof(HeaderData));
        outHeader.size = ntohs(outHeader.size);

        return isHeaderValid(packet, outHeader);
    }

    void UDPPacketRouter::handleAccept() const
    {
        _sink->onAccept();
    }

    void UDPPacketRouter::handleReject() const
    {
        _sink->onReject();
    }

    void UDPPacketRouter::handlePong() const
    {
        _sink->onPong();
    }

    void UDPPacketRouter::handleGameOver() const
    {
        _sink->onGameOver();
    }

    void UDPPacketRouter::handleSnapEntity(const uint8_t *payload, const size_t size) const
    {
        if (!payload || size < sizeof(SnapshotBatchHeader)) {
            std::cerr << "{UDPPacketRouter::handleSnapEntity} Snapshot batch too small\n";
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

    void UDPPacketRouter::handleScore(const uint8_t *payload, const size_t size) const
    {
        if (!payload || size != sizeof(ScoreData)) {
            std::cerr << "{UDPPacketRouter::handleScore} Dropped SCORE: bad size\n";
            return;
        }

        ScoreData scoreData{};
        std::memcpy(&scoreData, payload, sizeof(scoreData));
        const uint32_t score = ntohl(scoreData.score);

        _sink->onScore(score);
    }
} // namespace Ecs
