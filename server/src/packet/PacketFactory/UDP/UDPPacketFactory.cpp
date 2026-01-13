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
        const sockaddr_in &addr, uint32_t id, uint16_t amount, bool wasKilled) const noexcept
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

    std::shared_ptr<IPacket> UDPPacketFactory::createSnapshotPacket(
        const std::vector<SnapshotEntity> &entities, uint32_t serverTick) const noexcept
    {
        try {
            if (entities.size() > (std::numeric_limits<std::size_t>::max() - sizeof(SnapshotBatchHeader))
                    / sizeof(SnapshotEntityData)) {
                std::cerr << "{UDPPacketFactory::createSnapshotPacket} Too many entities in snapshot" << std::endl;
                return nullptr;
            }

            const auto totalSize = sizeof(SnapshotBatchHeader) + entities.size() * sizeof(SnapshotEntityData);

            if (totalSize > std::numeric_limits<uint16_t>::max()) {
                std::cerr << "{UDPPacketFactory::createSnapshotPacket} Snapshot packet size exceeds limit" << std::endl;
                return nullptr;
            }

            auto packet = _packet->newPacket();
            if (!packet) {
                std::cerr << "{UDPPacketFactory::createSnapshotPacket} Failed to create new packet" << std::endl;
                return nullptr;
            }

            if (totalSize > packet->capacity())
                throw FactoryError("{UDPPacketFactory::createSnapshotPacket} Snapshot too large");

            uint8_t *buf = packet->buffer();
            if (!buf)
                throw FactoryError("{UDPPacketFactory::createSnapshotPacket} Null buffer");

            SnapshotBatchHeader hdr{};
            hdr.header = makeHeader(Protocol::UDP::SNAPSHOT, VERSION, static_cast<uint16_t>(totalSize));
            hdr.count = htons(static_cast<uint16_t>(entities.size()));
            hdr.serverTick = htonl(serverTick);

            std::memcpy(buf, &hdr, sizeof(hdr));
            std::size_t offset = sizeof(hdr);

            for (const auto &[id, x, y, spriteId] : entities) {
                SnapshotEntityData packed{};

                packed.id = htonl(static_cast<uint32_t>(id));
                packed.x = static_cast<int16_t>(htons(static_cast<uint16_t>(static_cast<int16_t>(x))));
                packed.y = static_cast<int16_t>(htons(static_cast<uint16_t>(static_cast<int16_t>(y))));
                packed.spriteId = static_cast<uint8_t>(spriteId);

                std::memcpy(buf + offset, &packed, sizeof(packed));
                offset += sizeof(packed);
            }

            packet->setSize(totalSize);
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{UDPPacketFactory::createSnapshotPacket} " << e.what() << std::endl;
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

} // namespace Net::Factory
