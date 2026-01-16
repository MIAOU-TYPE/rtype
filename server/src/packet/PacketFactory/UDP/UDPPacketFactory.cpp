/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPPacketFactory
*/

#include "UDPPacketFactory.hpp"

namespace
{
    [[nodiscard]] int16_t htons_i16(const int16_t v) noexcept
    {
        return static_cast<int16_t>(htons(static_cast<uint16_t>(v)));
    }
} // namespace

namespace Net::Factory
{

    UDPPacketFactory::UDPPacketFactory(const std::shared_ptr<IPacket> &packet)
    {
        if (!packet)
            throw FactoryError("{UDPPacketFactory::UDPPacketFactory} Invalid IPacket pointer");
        _packet = packet;
    }

    HeaderData UDPPacketFactory::makeHeader(const uint8_t type, const uint8_t version, uint16_t size) noexcept
    {
        static uint32_t sequenceCounter = 1;

        HeaderData header{};
        std::memcpy(header.magic, kPacketMagic, sizeof(kPacketMagic));
        header.type = type;
        header.version = version;
        header.size = htons(size);
        header.sequence = htonl(sequenceCounter);
        sequenceCounter++;
        return header;
    }

    std::shared_ptr<IPacket> UDPPacketFactory::makeDefault(const sockaddr_in &addr, const uint8_t flag) const noexcept
    {
        DefaultData defaultPacket;
        defaultPacket.header = makeHeader(flag, VERSION, sizeof(DefaultData));

        try {
            auto packet = makePacket<DefaultData>(addr, defaultPacket);
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::makeDefault} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IPacket> UDPPacketFactory::makeDamage(
        const sockaddr_in &addr, uint32_t id, uint16_t amount, const bool wasKilled) const noexcept
    {
        DamageData damageData;
        damageData.header = makeHeader(Protocol::UDP::DAMAGE_EVENT, VERSION, sizeof(DamageData));
        damageData.id = htonl(id);
        damageData.amount = htons(amount);
        damageData.wasKilled = wasKilled;

        try {
            auto packet = makePacket<DamageData>(addr, damageData);
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::makeDamage} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IPacket> UDPPacketFactory::createScorePacket(const sockaddr_in &addr, uint32_t score) const noexcept
    {
        ScoreData scoreData;
        scoreData.header = makeHeader(Protocol::UDP::SCORE, VERSION, sizeof(ScoreData));
        scoreData.score = htons(static_cast<uint16_t>(score));
        try {
            auto packet = makePacket<ScoreData>(addr, scoreData);
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::createScorePacket} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IPacket> UDPPacketFactory::createDestroyEntityPacket(
        const size_t entityId, const bool wasKilled) const noexcept
    {
        try {
            auto packet = _packet->newPacket();
            if (!packet) {
                std::cerr << "{UDPPacketFactory::createSnapshotPacket} Failed to create new packet" << std::endl;
                return nullptr;
            }

            uint8_t *buf = packet->buffer();
            if (!buf)
                throw FactoryError("{UDPPacketFactory::createDestroyEntityPacket} Null buffer");

            DestroyData destroyData;
            destroyData.header = makeHeader(Protocol::UDP::DESTROY_ENTITY, VERSION, sizeof(DestroyData));
            destroyData.id = htonl(static_cast<uint32_t>(entityId));
            destroyData.wasKilled = wasKilled;
            std::memcpy(buf, &destroyData, sizeof(DestroyData));
            packet->setSize(sizeof(DestroyData));
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::createDestroyEntityPacket} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IPacket> UDPPacketFactory::createAcceptPacket(
        const sockaddr_in &addr, const size_t entityId) const noexcept
    {
        AcceptData acceptData;
        acceptData.header = makeHeader(Protocol::UDP::ACCEPT, VERSION, sizeof(AcceptData));
        acceptData.netPlayerId = htonl(static_cast<uint32_t>(entityId));

        try {
            auto packet = makePacket<AcceptData>(addr, acceptData);
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::createAcceptPacket} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IPacket> UDPPacketFactory::createHealthPacket(
        const sockaddr_in &addr, const uint16_t currentLife, const uint16_t maxLife) const noexcept
    {
        HealthData healthData;
        healthData.header = makeHeader(Protocol::UDP::HEALTH, VERSION, sizeof(HealthData));
        healthData.currentLife = htons(currentLife);
        healthData.maxLife = htons(maxLife);

        try {
            auto packet = makePacket<HealthData>(addr, healthData);
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::createHealthPacket} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::optional<UDPPacketFactory::ChunkSizes> UDPPacketFactory::computeChunkSizes(
        const size_t maxPacketBytes) noexcept
    {
        if (maxPacketBytes
            <= std::max(sizeof(SnapshotCompressedHeader), sizeof(SnapshotBatchHeader)) + sizeof(SnapshotEntityData))
            return std::nullopt;

        const size_t maxCompBytes = maxPacketBytes - sizeof(SnapshotCompressedHeader);

        const size_t maxEntitiesPerPkt =
            (maxPacketBytes - sizeof(SnapshotCompressedHeader)) / sizeof(SnapshotEntityData);

        if (maxEntitiesPerPkt == 0)
            return std::nullopt;

        return ChunkSizes{maxPacketBytes, maxCompBytes, maxEntitiesPerPkt};
    }

    void UDPPacketFactory::packEntitiesToRaw(std::vector<char> &rawBuf, const std::vector<SnapshotEntity> &entities,
        const size_t cursorEntity, const size_t count)
    {
        const size_t rawSize = count * sizeof(SnapshotEntityData);
        rawBuf.resize(rawSize);

        size_t off = 0;
        for (size_t i = 0; i < count; ++i) {
            const auto &[id, x, y, z, spriteId] = entities.at(cursorEntity + i);

            SnapshotEntityData packed{};
            packed.id = htonl(static_cast<uint32_t>(id));
            packed.x = htons_i16(static_cast<int16_t>(x));
            packed.y = htons_i16(static_cast<int16_t>(y));
            packed.z = z;
            packed.spriteId = static_cast<uint8_t>(spriteId);

            std::memcpy(rawBuf.data() + off, &packed, sizeof(packed));
            off += sizeof(packed);
        }
    }

    bool UDPPacketFactory::tryCompress(std::vector<char> &compBuf, const std::vector<char> &rawBuf,
        const size_t rawSize, const size_t wireUnc, const ChunkSizes &sz, size_t &outCompSize) const
    {
        const int maxDst = LZ4_compressBound(static_cast<int>(rawSize));
        compBuf.resize(static_cast<size_t>(maxDst));

        const int compSizeI = LZ4_compress_default(rawBuf.data(), compBuf.data(), static_cast<int>(rawSize), maxDst);

        if (compSizeI <= 0)
            return false;

        const auto compSize = static_cast<size_t>(compSizeI);
        const size_t wireComp = sizeof(SnapshotCompressedHeader) + compSize;

        const bool compFits = (compSize <= sz.maxCompBytes) && (wireComp <= sz.maxPacketBytes);

        if (const bool compWorth = compFits && (wireComp < wireUnc); !compWorth)
            return false;

        outCompSize = compSize;
        return true;
    }

    std::shared_ptr<IPacket> UDPPacketFactory::buildRawPacket(
        const std::vector<char> &rawBuf, const size_t rawSize, const uint16_t count, const uint32_t serverTick) const
    {
        const size_t wireUnc = sizeof(SnapshotBatchHeader) + rawSize;

        if (wireUnc > std::numeric_limits<uint16_t>::max())
            throw FactoryError("{UDPPacketFactory::createSnapshotPackets} raw packet too large for uint16");

        auto packet = _packet->newPacket();
        if (!packet)
            throw FactoryError("{UDPPacketFactory::createSnapshotPackets} Failed to create new packet");
        if (wireUnc > packet->capacity())
            throw FactoryError("{UDPPacketFactory::createSnapshotPackets} Packet capacity too small");

        uint8_t *buf = packet->buffer();
        if (!buf)
            throw FactoryError("{UDPPacketFactory::createSnapshotPackets} Null buffer");

        SnapshotBatchHeader hdr{};
        hdr.header = makeHeader(Protocol::UDP::SNAPSHOT_RAW, VERSION, static_cast<uint16_t>(wireUnc));
        hdr.count = htons(count);
        hdr.serverTick = htonl(serverTick);
        hdr.chunkIndex = 0;
        hdr.chunkCount = 0;

        std::memcpy(buf, &hdr, sizeof(hdr));
        std::memcpy(buf + sizeof(hdr), rawBuf.data(), rawSize);

        packet->setSize(wireUnc);
        return packet;
    }

    std::shared_ptr<IPacket> UDPPacketFactory::buildCompressedPacket(const std::vector<char> &compBuf,
        const size_t rawSize, const size_t compSize, const uint16_t count, const uint32_t serverTick) const
    {
        const size_t wireComp = sizeof(SnapshotCompressedHeader) + compSize;

        if (wireComp > std::numeric_limits<uint16_t>::max())
            throw FactoryError("{UDPPacketFactory::createSnapshotPackets} comp packet too large for uint16");

        auto packet = _packet->newPacket();
        if (!packet)
            throw FactoryError("{UDPPacketFactory::createSnapshotPackets} Failed to create new packet");
        if (wireComp > packet->capacity())
            throw FactoryError("{UDPPacketFactory::createSnapshotPackets} Packet capacity too small");

        uint8_t *buf = packet->buffer();
        if (!buf)
            throw FactoryError("{UDPPacketFactory::createSnapshotPackets} Null buffer");

        SnapshotCompressedHeader hdr{};
        hdr.header = makeHeader(Protocol::UDP::SNAPSHOT_COMPRESSED, VERSION, static_cast<uint16_t>(wireComp));
        hdr.count = htons(count);
        hdr.serverTick = htonl(serverTick);
        hdr.chunkIndex = 0;
        hdr.chunkCount = 0;
        hdr.rawSize = htons(static_cast<uint16_t>(rawSize));
        hdr.compSize = htons(static_cast<uint16_t>(compSize));

        std::memcpy(buf, &hdr, sizeof(hdr));
        std::memcpy(buf + sizeof(hdr), compBuf.data(), compSize);

        packet->setSize(wireComp);
        return packet;
    }

    void UDPPacketFactory::patchChunkInfo(
        const BuiltPkt &bp, const uint16_t chunkIndexNet, const uint16_t chunkCountNet) noexcept
    {
        if (!bp.pkt)
            return;

        uint8_t *buf = bp.pkt->buffer();
        if (!buf)
            return;

        if (bp.compressed) {
            SnapshotCompressedHeader hdr{};
            std::memcpy(&hdr, buf, sizeof(hdr));
            hdr.chunkIndex = chunkIndexNet;
            hdr.chunkCount = chunkCountNet;
            std::memcpy(buf, &hdr, sizeof(hdr));
        } else {
            SnapshotBatchHeader hdr{};
            std::memcpy(&hdr, buf, sizeof(hdr));
            hdr.chunkIndex = chunkIndexNet;
            hdr.chunkCount = chunkCountNet;
            std::memcpy(buf, &hdr, sizeof(hdr));
        }
    }

    std::optional<UDPPacketFactory::BuiltPkt> UDPPacketFactory::buildOneChunk(
        const std::vector<SnapshotEntity> &entities, size_t &cursorEntity, const ChunkSizes &sz,
        const uint32_t serverTick, std::vector<char> &rawBuf, std::vector<char> &compBuf) const
    {
        size_t count = std::min(sz.maxEntitiesPerPkt, entities.size() - cursorEntity);

        for (;;) {
            if (count == 0) {
                std::cerr << "{UDPPacketFactory::createSnapshotPackets} cannot fit even 1 entity in chunk\n";
                return std::nullopt;
            }

            const size_t rawSize = count * sizeof(SnapshotEntityData);
            if (rawSize > std::numeric_limits<uint16_t>::max()) {
                count /= 2;
                continue;
            }

            packEntitiesToRaw(rawBuf, entities, cursorEntity, count);

            const size_t wireUnc = sizeof(SnapshotBatchHeader) + rawSize;
            if (wireUnc > sz.maxPacketBytes) {
                count /= 2;
                continue;
            }

            size_t compSize = 0;
            const bool useComp = tryCompress(compBuf, rawBuf, rawSize, wireUnc, sz, compSize);

            BuiltPkt out{};
            out.compressed = useComp;
            out.count = static_cast<uint16_t>(count);

            if (useComp) {
                out.pkt = buildCompressedPacket(compBuf, rawSize, compSize, out.count, serverTick);
            } else {
                out.pkt = buildRawPacket(rawBuf, rawSize, out.count, serverTick);
            }

            cursorEntity += count;
            return out;
        }
    }

    std::vector<std::shared_ptr<IPacket>> UDPPacketFactory::createSnapshotPackets(
        const std::vector<SnapshotEntity> &entities, const uint32_t serverTick,
        const size_t maxPacketBytes) const noexcept
    {
        try {
            std::vector<std::shared_ptr<IPacket>> out;
            if (entities.empty())
                return out;

            const auto szOpt = computeChunkSizes(maxPacketBytes);
            if (!szOpt)
                return out;
            const ChunkSizes sz = *szOpt;

            std::vector<BuiltPkt> built;
            built.reserve((entities.size() + sz.maxEntitiesPerPkt - 1) / sz.maxEntitiesPerPkt);

            std::vector<char> rawBuf;
            std::vector<char> compBuf;

            size_t cursorEntity = 0;
            while (cursorEntity < entities.size()) {
                auto bp = buildOneChunk(entities, cursorEntity, sz, serverTick, rawBuf, compBuf);
                if (!bp)
                    return {};
                built.push_back(std::move(*bp));
            }

            const size_t finalChunkCount = built.size();
            if (finalChunkCount > std::numeric_limits<uint16_t>::max())
                return {};

            out.reserve(finalChunkCount);

            const uint16_t cnt16 = htons(static_cast<uint16_t>(finalChunkCount));
            for (size_t i = 0; i < finalChunkCount; ++i) {
                patchChunkInfo(built.at(i), htons(static_cast<uint16_t>(i)), cnt16);
                if (built.at(i).pkt)
                    out.push_back(std::move(built.at(i).pkt));
            }
            return out;
        } catch (const std::exception &e) {
            std::cerr << "{UDPPacketFactory::createSnapshotPackets} " << e.what() << "\n";
            return {};
        }
    }

} // namespace Net::Factory
