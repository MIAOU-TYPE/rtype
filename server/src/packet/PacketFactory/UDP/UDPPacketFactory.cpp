/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPPacketFactory
*/

#include "UDPPacketFactory.hpp"

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
        const sockaddr_in &addr, uint32_t id, uint16_t amount) const noexcept
    {
        DamageData damageData;
        damageData.header = makeHeader(Protocol::UDP::DAMAGE_EVENT, VERSION, sizeof(DamageData));
        damageData.id = htonl(id);
        damageData.amount = htons(amount);

        try {
            auto packet = makePacket<DamageData>(addr, damageData);
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::makeDamage} " << e.what() << std::endl;
            return nullptr;
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

            if (maxPacketBytes
                <= std::max(sizeof(SnapshotCompressedHeader), sizeof(SnapshotBatchHeader)) + sizeof(SnapshotEntityData))
                return out;

            const size_t maxCompBytes = maxPacketBytes - sizeof(SnapshotCompressedHeader);

            const size_t maxEntitiesPerPkt =
                (maxPacketBytes - sizeof(SnapshotCompressedHeader)) / sizeof(SnapshotEntityData);

            if (maxEntitiesPerPkt == 0)
                return out;

            struct BuiltPkt {
                std::shared_ptr<IPacket> pkt;
                bool compressed;
                uint16_t count;
            };

            std::vector<BuiltPkt> built;
            built.reserve((entities.size() + maxEntitiesPerPkt - 1) / maxEntitiesPerPkt);

            std::vector<char> rawBuf;
            std::vector<char> compBuf;

            size_t cursorEntity = 0;

            while (cursorEntity < entities.size()) {
                size_t count = std::min(maxEntitiesPerPkt, entities.size() - cursorEntity);

                for (;;) {
                    if (count == 0) {
                        std::cerr << "{UDPPacketFactory::createSnapshotPackets} cannot fit even 1 entity in chunk\n";
                        return {};
                    }

                    const size_t rawSize = count * sizeof(SnapshotEntityData);
                    if (rawSize > std::numeric_limits<uint16_t>::max()) {
                        count /= 2;
                        continue;
                    }

                    rawBuf.resize(rawSize);
                    size_t off = 0;
                    for (size_t i = 0; i < count; ++i) {
                        const auto &[id, x, y, spriteId] = entities.at(cursorEntity + i);

                        SnapshotEntityData packed{};
                        packed.id = htonl(static_cast<uint32_t>(id));
                        packed.x = htons(static_cast<uint16_t>(x));
                        packed.y = htons(static_cast<uint16_t>(y));
                        packed.spriteId = static_cast<uint8_t>(spriteId);

                        std::memcpy(rawBuf.data() + off, &packed, sizeof(packed));
                        off += sizeof(packed);
                    }

                    const size_t wireUnc = sizeof(SnapshotBatchHeader) + rawSize;

                    if (wireUnc > maxPacketBytes) {
                        count /= 2;
                        continue;
                    }

                    const int maxDst = LZ4_compressBound(static_cast<int>(rawSize));
                    compBuf.resize(static_cast<size_t>(maxDst));

                    const int compSizeI =
                        LZ4_compress_default(rawBuf.data(), compBuf.data(), static_cast<int>(rawSize), maxDst);

                    size_t compSize = 0;
                    bool canUseComp = false;

                    if (compSizeI > 0) {
                        compSize = static_cast<size_t>(compSizeI);

                        const size_t wireComp = sizeof(SnapshotCompressedHeader) + compSize;
                        const bool compFits = (compSize <= maxCompBytes) && (wireComp <= maxPacketBytes);
                        const bool compWorth = compFits && (wireComp < wireUnc);

                        canUseComp = compWorth;
                    }

                    if (!canUseComp) {
                        if (wireUnc > std::numeric_limits<uint16_t>::max())
                            throw FactoryError(
                                "{UDPPacketFactory::createSnapshotPackets} raw packet too large for uint16");

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
                        hdr.count = htons(static_cast<uint16_t>(count));
                        hdr.serverTick = htonl(serverTick);

                        hdr.chunkIndex = 0;
                        hdr.chunkCount = 0;

                        std::memcpy(buf, &hdr, sizeof(hdr));
                        std::memcpy(buf + sizeof(hdr), rawBuf.data(), rawSize);

                        packet->setSize(wireUnc);
                        built.push_back(BuiltPkt{std::move(packet), false, static_cast<uint16_t>(count)});

                        cursorEntity += count;
                        break;
                    } else {
                        const size_t wireComp = sizeof(SnapshotCompressedHeader) + compSize;

                        if (wireComp > std::numeric_limits<uint16_t>::max())
                            throw FactoryError(
                                "{UDPPacketFactory::createSnapshotPackets} comp packet too large for uint16");

                        auto packet = _packet->newPacket();
                        if (!packet)
                            throw FactoryError("{UDPPacketFactory::createSnapshotPackets} Failed to create new packet");
                        if (wireComp > packet->capacity())
                            throw FactoryError("{UDPPacketFactory::createSnapshotPackets} Packet capacity too small");

                        uint8_t *buf = packet->buffer();
                        if (!buf)
                            throw FactoryError("{UDPPacketFactory::createSnapshotPackets} Null buffer");

                        SnapshotCompressedHeader hdr{};
                        hdr.header =
                            makeHeader(Protocol::UDP::SNAPSHOT_COMPRESSED, VERSION, static_cast<uint16_t>(wireComp));
                        hdr.count = htons(static_cast<uint16_t>(count));
                        hdr.serverTick = htonl(serverTick);

                        hdr.chunkIndex = 0;
                        hdr.chunkCount = 0;

                        hdr.rawSize = htons(static_cast<uint16_t>(rawSize));
                        hdr.compSize = htons(static_cast<uint16_t>(compSize));

                        std::memcpy(buf, &hdr, sizeof(hdr));
                        std::memcpy(buf + sizeof(hdr), compBuf.data(), compSize);

                        packet->setSize(wireComp);
                        built.push_back(BuiltPkt{std::move(packet), true, static_cast<uint16_t>(count)});

                        cursorEntity += count;
                        break;
                    }
                }
            }

            const size_t finalChunkCount = built.size();
            if (finalChunkCount > std::numeric_limits<uint16_t>::max())
                return {};

            out.reserve(finalChunkCount);

            for (size_t i = 0; i < finalChunkCount; ++i) {
                auto &bp = built.at(i);
                if (!bp.pkt)
                    continue;

                uint8_t *buf = bp.pkt->buffer();
                if (!buf)
                    continue;

                const uint16_t idx16 = htons(static_cast<uint16_t>(i));
                const uint16_t cnt16 = htons(static_cast<uint16_t>(finalChunkCount));

                if (bp.compressed) {
                    SnapshotCompressedHeader hdr{};
                    std::memcpy(&hdr, buf, sizeof(hdr));
                    hdr.chunkIndex = idx16;
                    hdr.chunkCount = cnt16;
                    std::memcpy(buf, &hdr, sizeof(hdr));
                } else {
                    SnapshotBatchHeader hdr{};
                    std::memcpy(&hdr, buf, sizeof(hdr));
                    hdr.chunkIndex = idx16;
                    hdr.chunkCount = cnt16;
                    std::memcpy(buf, &hdr, sizeof(hdr));
                }

                out.push_back(std::move(bp.pkt));
            }

            return out;
        } catch (const std::exception &e) {
            std::cerr << "{UDPPacketFactory::createSnapshotPackets} " << e.what() << "\n";
            return {};
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

    std::shared_ptr<IPacket> UDPPacketFactory::createDestroyEntityPacket(const size_t entityId) const noexcept
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
            std::memcpy(buf, &destroyData, sizeof(DestroyData));
            packet->setSize(sizeof(DestroyData));
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::createDestroyEntityPacket} " << e.what() << std::endl;
            return nullptr;
        }
    }

} // namespace Net::Factory
