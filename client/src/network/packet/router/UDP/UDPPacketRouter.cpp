/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPPacketRouter
*/

#include "UDPPacketRouter.hpp"

namespace
{
    [[nodiscard]] bool isRecent(const uint32_t a, const uint32_t b) noexcept
    {
        return static_cast<int32_t>(a - b) >= 0;
    }
} // namespace

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

        dispatchPacket(header, packet->buffer(), packet->size());
    }

    void UDPPacketRouter::dispatchPacket(
        const HeaderData &header, const uint8_t *payload, const std::size_t payloadSize) const
    {
        switch (header.type) {
            case Net::Protocol::UDP::ACCEPT:
                if (payloadSize == sizeof(DefaultData))
                    handleAccept();
                break;

            case Net::Protocol::UDP::REJECT:
                if (payloadSize == sizeof(DefaultData))
                    handleReject();
                break;

            case Net::Protocol::UDP::GAME_OVER:
                if (payloadSize == sizeof(DefaultData))
                    handleGameOver();
                break;

            case Net::Protocol::UDP::PONG:
                if (payloadSize == sizeof(DefaultData))
                    handlePong();
                break;

            case Net::Protocol::UDP::SNAPSHOT_RAW: handleSnapEntityRaw(payload, payloadSize); break;

            case Net::Protocol::UDP::SNAPSHOT_COMPRESSED: handleSnapEntityCompressed(payload, payloadSize); break;

            case Net::Protocol::UDP::DAMAGE_EVENT: handleDamage(payload, payloadSize); break;

            case Net::Protocol::UDP::SCORE: handleScore(payload, payloadSize); break;

            case Net::Protocol::UDP::DESTROY_ENTITY: handleDestroy(payload, payloadSize); break;

            default:
                std::cerr << "{UDPPacketRouter::dispatchPacket} Unknown packet type: " << static_cast<int>(header.type)
                          << std::endl;
                break;
        }
    }

    bool UDPPacketRouter::isHeaderValid(const Net::IPacket &packet, const HeaderData &header)
    {
        static uint32_t lastSequence = 0;

        if (packet.size() < sizeof(HeaderData)) {
            std::cerr << "{UDPPacketRouter::isHeaderValid} Dropped: packet too small\n";
            return false;
        }
        if (std::memcmp(header.magic, kPacketMagic, 4) != 0) {
            std::cerr << "{UDPPacketRouter::isHeaderValid} Dropped: bad magic\n";
            return false;
        }

        if (!isRecent(header.sequence, lastSequence)) {
            std::cerr << "{UDPPacketRouter::isHeaderValid} Dropped: out-of-order packet (sequence=" << header.sequence
                      << ", last=" << lastSequence << ")\n";
            return false;
        }

        if (header.version != PROTOCOL_VERSION) {
            std::cerr << "{UDPPacketRouter::isHeaderValid} Dropped: wrong protocol version "
                      << static_cast<int>(header.version) << " (expected " << static_cast<int>(PROTOCOL_VERSION)
                      << ")\n";
            return false;
        }
        if (const uint16_t declaredSize = header.size; declaredSize != static_cast<uint16_t>(packet.size())) {
            std::cerr << "{UDPPacketRouter::isHeaderValid} Dropped: size mismatch (header=" << declaredSize
                      << ", actual=" << packet.size() << ")\n";
            return false;
        }
        lastSequence = header.sequence;
        return true;
    }

    bool UDPPacketRouter::isPacketValid(const std::shared_ptr<Net::IPacket> &packet) noexcept
    {
        return packet && packet->size() >= sizeof(HeaderData);
    }

    bool UDPPacketRouter::extractHeader(const Net::IPacket &packet, HeaderData &outHeader) noexcept
    {
        if (!packet.buffer() || packet.size() < sizeof(HeaderData))
            return false;

        std::memcpy(&outHeader, packet.buffer(), sizeof(HeaderData));
        outHeader.size = ntohs(outHeader.size);
        outHeader.sequence = ntohl(outHeader.sequence);

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

    void UDPPacketRouter::handleSnapEntityRaw(const uint8_t *payload, const size_t size) const
    {
        if (!payload || size < sizeof(SnapshotBatchHeader)) {
            std::cerr << "{UDPPacketRouter::handleSnapEntityRaw} Snapshot chunk too small\n";
            return;
        }

        SnapshotBatchHeader h{};
        std::memcpy(&h, payload, sizeof(h));

        const uint16_t count = ntohs(h.count);
        const uint32_t serverTick = ntohl(h.serverTick);
        const uint16_t chunkIndex = ntohs(h.chunkIndex);
        const uint16_t chunkCount = ntohs(h.chunkCount);

        if (chunkCount == 0 || chunkIndex >= chunkCount) {
            std::cerr << "{UDPPacketRouter::handleSnapEntityRaw} Bad chunk meta\n";
            return;
        }

        const size_t expectedMin =
            sizeof(SnapshotBatchHeader) + static_cast<size_t>(count) * sizeof(SnapshotEntityData);
        if (size < expectedMin) {
            std::cerr << "{UDPPacketRouter::handleSnapEntityRaw} Truncated raw chunk\n";
            return;
        }

        const auto now = std::chrono::steady_clock::now();
        purgeExpired(now);

        auto &acc = getOrResetAcc(serverTick, chunkCount, now);
        if (!acceptChunk(acc, chunkIndex))
            return;

        const uint8_t *raw = payload + sizeof(SnapshotBatchHeader);
        appendEntitiesFromRaw(acc, raw, count);

        if (!isComplete(acc))
            return;

        _sink->onSnapshot(serverTick, acc.merged);
        _pending.erase(serverTick);
    }

    void UDPPacketRouter::handleSnapEntityCompressed(const uint8_t *payload, const size_t size) const
    {
        if (!payload || size < sizeof(SnapshotCompressedHeader)) {
            std::cerr << "{UDPPacketRouter::handleSnapEntityCompressed} Snapshot chunk too small\n";
            return;
        }

        SnapshotCompressedHeader h{};
        std::memcpy(&h, payload, sizeof(h));

        const uint16_t count = ntohs(h.count);
        const uint32_t serverTick = ntohl(h.serverTick);
        const uint16_t chunkIndex = ntohs(h.chunkIndex);
        const uint16_t chunkCount = ntohs(h.chunkCount);
        const uint16_t rawSize = ntohs(h.rawSize);
        const uint16_t compSize = ntohs(h.compSize);

        if (chunkCount == 0 || chunkIndex >= chunkCount) {
            std::cerr << "{UDPPacketRouter::handleSnapEntityCompressed} Bad chunk meta\n";
            return;
        }

        if (const size_t expectedRaw = static_cast<size_t>(count) * sizeof(SnapshotEntityData);
            expectedRaw != rawSize) {
            std::cerr << "{UDPPacketRouter::handleSnapEntityCompressed} rawSize mismatch\n";
            return;
        }

        std::vector<char> raw;
        if (!decompressSnapshotPayload(payload, size, rawSize, compSize, raw)) {
            std::cerr << "{UDPPacketRouter::handleSnapEntityCompressed} LZ4_decompress_safe failed\n";
            return;
        }

        const auto now = std::chrono::steady_clock::now();
        purgeExpired(now);

        auto &acc = getOrResetAcc(serverTick, chunkCount, now);
        if (!acceptChunk(acc, chunkIndex))
            return;

        appendEntitiesFromRaw(acc, reinterpret_cast<const uint8_t *>(raw.data()), count);

        if (!isComplete(acc))
            return;

        _sink->onSnapshot(serverTick, acc.merged);
        _pending.erase(serverTick);
    }

    void UDPPacketRouter::handleScore(const uint8_t *payload, const size_t size) const
    {
        if (!payload || size != sizeof(ScoreData)) {
            std::cerr << "{UDPPacketRouter::handleScore} Dropped SCORE: bad size\n";
            return;
        }

        ScoreData scoreData{};
        std::memcpy(&scoreData, payload, sizeof(scoreData));
        const uint32_t score = ntohs(scoreData.score);
        _sink->onScore(score);
    }

    void UDPPacketRouter::handleDamage(const uint8_t *payload, const size_t size) const
    {
        if (!payload || size != sizeof(DamageData)) {
            std::cerr << "{UDPPacketRouter::handleDamage} Dropped DAMAGE_EVENT: bad size\n";
            return;
        }

        DamageData damageData{};
        std::memcpy(&damageData, payload, sizeof(damageData));
        const uint32_t targetId = ntohl(damageData.id);
        const bool wasKilled = damageData.wasKilled != 0;

        _sink->onDamage(targetId, wasKilled);
    }

    void UDPPacketRouter::handleDestroy(const uint8_t *payload, const size_t size) const
    {
        if (!payload || size != sizeof(DestroyData)) {
            std::cerr << "{UDPPacketRouter::handleDestroy} Dropped DESTROY: bad size\n";
            return;
        }

        DestroyData destroyData{};
        std::memcpy(&destroyData, payload, sizeof(destroyData));
        const uint32_t entityId = ntohl(destroyData.id);
        const bool wasKilled = destroyData.wasKilled != 0;
        _sink->onDestroy(entityId, wasKilled);
    }

    void UDPPacketRouter::purgeExpired(const std::chrono::steady_clock::time_point now) const
    {
        for (auto it = _pending.begin(); it != _pending.end();) {
            if (now - it->second.t0 > PendingTTL)
                it = _pending.erase(it);
            else
                ++it;
        }
    }

    UDPPacketRouter::PendingSnapshot &UDPPacketRouter::getOrResetAcc(
        const uint32_t serverTick, const uint16_t chunkCount, const std::chrono::steady_clock::time_point now) const
    {
        auto &acc = _pending[serverTick];

        const bool needReset =
            (acc.chunkCount == 0) || (acc.chunkCount != chunkCount) || (acc.received.size() != chunkCount);

        if (needReset) {
            acc.chunkCount = chunkCount;
            acc.received.assign(chunkCount, 0);
            acc.merged.clear();
            acc.t0 = now;
        }
        return acc;
    }

    bool UDPPacketRouter::acceptChunk(PendingSnapshot &acc, const uint16_t chunkIndex)
    {
        if (chunkIndex >= acc.chunkCount || acc.received.at(chunkIndex))
            return false;
        acc.received[chunkIndex] = 1;
        return true;
    }

    bool UDPPacketRouter::isComplete(const PendingSnapshot &acc)
    {
        for (uint16_t i = 0; i < acc.chunkCount; ++i) {
            if (i >= acc.received.size() || !acc.received.at(i))
                return false;
        }
        return true;
    }

    void UDPPacketRouter::appendEntitiesFromRaw(PendingSnapshot &acc, const uint8_t *raw, const uint16_t count)
    {
        const uint8_t *cursor = raw;
        for (uint16_t i = 0; i < count; ++i) {
            SnapshotEntityData d{};
            std::memcpy(&d, cursor, sizeof(d));

            SnapshotEntity e{};
            e.id = ntohl(d.id);
            e.x = static_cast<float>(static_cast<int16_t>(ntohs(d.x)));
            e.y = static_cast<float>(static_cast<int16_t>(ntohs(d.y)));
            e.z = d.z;
            e.spriteId = d.spriteId;

            acc.merged.push_back(e);
            cursor += sizeof(SnapshotEntityData);
        }
    }

    bool UDPPacketRouter::decompressSnapshotPayload(const uint8_t *payload, const size_t size, const uint16_t rawSize,
        const uint16_t compSize, std::vector<char> &outRaw)
    {
        if (const size_t expectedMin = sizeof(SnapshotCompressedHeader) + static_cast<size_t>(compSize);
            !payload || size < expectedMin)
            return false;

        outRaw.resize(static_cast<size_t>(rawSize));

        const auto comp = reinterpret_cast<const char *>(payload + sizeof(SnapshotCompressedHeader));
        const int decoded =
            LZ4_decompress_safe(comp, outRaw.data(), static_cast<int>(compSize), static_cast<int>(rawSize));

        return std::cmp_equal(decoded, rawSize);
    }

} // namespace Ecs
