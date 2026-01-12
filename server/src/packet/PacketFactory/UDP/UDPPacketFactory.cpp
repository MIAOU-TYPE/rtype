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
        const std::size_t maxPacketBytes) const noexcept
    {
        try {
            std::vector<std::shared_ptr<IPacket>> out;
            if (maxPacketBytes <= sizeof(SnapshotBatchHeader) + sizeof(SnapshotEntityData))
                return out;

            const std::size_t maxEntitiesPerPkt =
                (maxPacketBytes - sizeof(SnapshotBatchHeader)) / sizeof(SnapshotEntityData);

            if (maxEntitiesPerPkt == 0)
                return out;

            const std::size_t chunkCount = (entities.size() + maxEntitiesPerPkt - 1) / maxEntitiesPerPkt;

            if (chunkCount > std::numeric_limits<uint16_t>::max()) {
                std::cerr << "{UDPPacketFactory::createSnapshotPackets} Too many chunks\n";
                return {};
            }

            out.reserve(chunkCount);

            for (std::size_t chunkIndex = 0; chunkIndex < chunkCount; ++chunkIndex) {
                const std::size_t begin = chunkIndex * maxEntitiesPerPkt;
                const std::size_t end = std::min(begin + maxEntitiesPerPkt, entities.size());
                const std::size_t count = end - begin;

                const std::size_t totalSize = sizeof(SnapshotBatchHeader) + count * sizeof(SnapshotEntityData);

                if (totalSize > std::numeric_limits<uint16_t>::max())
                    throw FactoryError("{UDPPacketFactory::createSnapshotPackets} chunk too large for uint16 size");

                auto packet = _packet->newPacket();
                if (!packet)
                    throw FactoryError("{UDPPacketFactory::createSnapshotPackets} Failed to create new packet");

                if (totalSize > packet->capacity())
                    throw FactoryError("{UDPPacketFactory::createSnapshotPackets} Packet capacity too small");

                uint8_t *buf = packet->buffer();
                if (!buf)
                    throw FactoryError("{UDPPacketFactory::createSnapshotPackets} Null buffer");

                SnapshotBatchHeader hdr{};
                hdr.header = makeHeader(Protocol::UDP::SNAPSHOT, VERSION, static_cast<uint16_t>(totalSize));
                hdr.count = htons(static_cast<uint16_t>(count));
                hdr.serverTick = htonl(serverTick);
                hdr.chunkIndex = htons(static_cast<uint16_t>(chunkIndex));
                hdr.chunkCount = htons(static_cast<uint16_t>(chunkCount));

                std::memcpy(buf, &hdr, sizeof(hdr));
                std::size_t offset = sizeof(hdr);

                for (std::size_t i = begin; i < end; ++i) {
                    const auto &[id, x, y, spriteId] = entities.at(i);

                    SnapshotEntityData packed{};
                    packed.id = htonl(static_cast<uint32_t>(id));
                    packed.x = htons(static_cast<uint16_t>(x));
                    packed.y = htons(static_cast<uint16_t>(y));
                    packed.spriteId = static_cast<uint8_t>(spriteId);

                    std::memcpy(buf + offset, &packed, sizeof(packed));
                    offset += sizeof(packed);
                }

                packet->setSize(totalSize);
                out.push_back(std::move(packet));
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
