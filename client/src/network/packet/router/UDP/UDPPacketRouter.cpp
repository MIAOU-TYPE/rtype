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

            case Net::Protocol::UDP::SCORE: handleScore(payload, payloadSize); break;

            case Net::Protocol::UDP::DESTROY_ENTITY: handleDestroy(payload, payloadSize); break;

            default:
                std::cerr << "{UDPPacketRouter::dispatchPacket} Unknown packet type: " << static_cast<int>(header.type)
                          << '\n';
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
            std::cerr << "{UDPPacketRouter::isHeaderValid} Dropped: out-of-order/duplicate packet (sequence="
                      << header.sequence << ", last=" << lastSequence << ")\n";
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
        using clock = std::chrono::steady_clock;
        static constexpr auto PendingTTL = std::chrono::milliseconds(400);

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
            std::cerr << "{UDPPacketRouter::handleSnapEntityRaw} Bad chunk meta (idx=" << chunkIndex
                      << ", count=" << chunkCount << ")\n";
            return;
        }

        const size_t expectedMin =
            sizeof(SnapshotBatchHeader) + static_cast<size_t>(count) * sizeof(SnapshotEntityData);
        if (size < expectedMin) {
            std::cerr << "{UDPPacketRouter::handleSnapEntityRaw} Truncated raw chunk (size=" << size
                      << ", expected>=" << expectedMin << ")\n";
            return;
        }

        // TTL cleanup (drop old incomplete snapshots, do NOT emit partial here)
        const auto now = clock::now();
        for (auto it = _pending.begin(); it != _pending.end();) {
            if (now - it->second.t0 > PendingTTL)
                it = _pending.erase(it);
            else
                ++it;
        }

        auto &acc = _pending[serverTick];
        if (acc.chunkCount == 0) {
            acc.chunkCount = chunkCount;
            acc.received.assign(chunkCount, 0);
            acc.merged.clear();
            acc.t0 = now;
        } else if (acc.chunkCount != chunkCount) {
            // reset accumulator if meta changes
            acc.chunkCount = chunkCount;
            acc.received.assign(chunkCount, 0);
            acc.merged.clear();
            acc.t0 = now;
        }

        if (chunkIndex >= acc.chunkCount)
            return;

        if (acc.received[chunkIndex])
            return;
        acc.received[chunkIndex] = 1;

        const uint8_t *cursor = payload + sizeof(SnapshotBatchHeader);
        for (uint16_t i = 0; i < count; ++i) {
            SnapshotEntityData entityData{};
            std::memcpy(&entityData, cursor, sizeof(entityData));

            SnapshotEntity e{};
            e.id = ntohl(entityData.id);
            e.x = ntohs(entityData.x);
            e.y = ntohs(entityData.y);
            e.spriteId = entityData.spriteId;

            acc.merged.push_back(e);
            cursor += sizeof(SnapshotEntityData);
        }

        for (uint16_t i = 0; i < acc.chunkCount; ++i) {
            if (!acc.received[i])
                return;
        }

        _sink->onSnapshot(serverTick, acc.merged);
        _pending.erase(serverTick);
    }

    void UDPPacketRouter::handleSnapEntityCompressed(const uint8_t *payload, const size_t size) const
    {
        using clock = std::chrono::steady_clock;
        static constexpr auto PendingTTL = std::chrono::milliseconds(400);

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
            std::cerr << "{UDPPacketRouter::handleSnapEntityCompressed} Bad chunk meta (idx=" << chunkIndex
                      << ", count=" << chunkCount << ")\n";
            return;
        }

        const size_t expectedRaw = static_cast<size_t>(count) * sizeof(SnapshotEntityData);
        if (expectedRaw != rawSize) {
            std::cerr << "{UDPPacketRouter::handleSnapEntityCompressed} rawSize mismatch (rawSize=" << rawSize
                      << ", expected=" << expectedRaw << ")\n";
            return;
        }

        const size_t expectedMin = sizeof(SnapshotCompressedHeader) + static_cast<size_t>(compSize);
        if (size < expectedMin) {
            std::cerr << "{UDPPacketRouter::handleSnapEntityCompressed} Truncated compressed chunk (size=" << size
                      << ", expected>=" << expectedMin << ")\n";
            return;
        }

        const auto now = clock::now();
        for (auto it = _pending.begin(); it != _pending.end();) {
            if (now - it->second.t0 > PendingTTL)
                it = _pending.erase(it);
            else
                ++it;
        }

        auto &acc = _pending[serverTick];
        if (acc.chunkCount == 0) {
            acc.chunkCount = chunkCount;
            acc.received.assign(chunkCount, 0);
            acc.merged.clear();
            acc.t0 = now;
        } else if (acc.chunkCount != chunkCount) {
            acc.chunkCount = chunkCount;
            acc.received.assign(chunkCount, 0);
            acc.merged.clear();
            acc.t0 = now;
        }

        if (chunkIndex >= acc.chunkCount)
            return;

        if (acc.received[chunkIndex])
            return;
        acc.received[chunkIndex] = 1;

        const auto *comp = reinterpret_cast<const char *>(payload + sizeof(SnapshotCompressedHeader));
        std::vector<char> raw(static_cast<size_t>(rawSize));

        const int decoded =
            LZ4_decompress_safe(comp, raw.data(), static_cast<int>(compSize), static_cast<int>(rawSize));
        if (decoded != static_cast<int>(rawSize)) {
            std::cerr << "{UDPPacketRouter::handleSnapEntityCompressed} LZ4_decompress_safe failed\n";
            return;
        }

        const auto *cursor = reinterpret_cast<const uint8_t *>(raw.data());
        for (uint16_t i = 0; i < count; ++i) {
            SnapshotEntityData entityData{};
            std::memcpy(&entityData, cursor, sizeof(entityData));

            SnapshotEntity e{};
            e.id = ntohl(entityData.id);
            e.x = ntohs(entityData.x);
            e.y = ntohs(entityData.y);
            e.spriteId = entityData.spriteId;

            acc.merged.push_back(e);
            cursor += sizeof(SnapshotEntityData);
        }

        for (uint16_t i = 0; i < acc.chunkCount; ++i) {
            if (!acc.received[i])
                return;
        }

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

    void UDPPacketRouter::handleDestroy(const uint8_t *payload, const size_t size) const
    {
        if (!payload || size != sizeof(DestroyData)) {
            std::cerr << "{UDPPacketRouter::handleDestroy} Dropped DESTROY: bad size\n";
            return;
        }

        DestroyData destroyData{};
        std::memcpy(&destroyData, payload, sizeof(destroyData));
        const uint32_t entityId = ntohl(destroyData.id);
        _sink->onDestroy(entityId);
    }
} // namespace Ecs
