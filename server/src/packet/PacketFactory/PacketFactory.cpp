/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#include "PacketFactory.hpp"

namespace Net::Factory
{

    PacketFactory::PacketFactory(const std::shared_ptr<IPacket> &packet)
    {
        if (!packet)
            throw FactoryError("{PacketFactory::PacketFactory} Invalid IPacket pointer");
        _packet = packet;
    }

    HeaderData PacketFactory::makeHeader(const uint8_t type, const uint8_t version, uint16_t size) noexcept
    {
        HeaderData header;

        header.type = type;
        header.version = version;
        header.size = htons(size);
        return header;
    }

    std::shared_ptr<IPacket> PacketFactory::makeDefault(const sockaddr_in &addr, const uint8_t flag) const noexcept
    {
        DefaultData defaultPacket;
        defaultPacket.header = makeHeader(flag, VERSION, sizeof(DefaultData));

        try {
            auto packet = makePacket<DefaultData>(addr, defaultPacket);
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{PacketFactory::makeDefault} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IPacket> PacketFactory::makeDamage(
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
            std::cerr << "{PacketFactory::makeDamage} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IPacket> PacketFactory::createSnapshotPacket(
        const std::vector<SnapshotEntity> &entities) const noexcept
    {
        try {
            SnapshotBatchHeader header{};
            if (entities.size()
                > (std::numeric_limits<size_t>::max() - sizeof(SnapshotBatchHeader)) / sizeof(SnapshotEntityData)) {
                std::cerr << "{PacketFactory::createSnapshotPacket} Too many entities in snapshot" << std::endl;
                return nullptr;
            }

            const auto totalSize = sizeof(SnapshotBatchHeader) + entities.size() * sizeof(SnapshotEntityData);
            if (totalSize > std::numeric_limits<uint16_t>::max()) {
                std::cerr << "{PacketFactory::createSnapshotPacket} Snapshot packet size exceeds limit" << std::endl;
                return nullptr;
            }

            header.header = makeHeader(Protocol::UDP::SNAPSHOT, VERSION, static_cast<uint16_t>(totalSize));
            header.count = htons(static_cast<uint16_t>(entities.size()));

            auto packet = _packet->newPacket();
            if (!packet) {
                std::cerr << "{PacketFactory::createSnapshotPacket} Failed to create new packet" << std::endl;
                return nullptr;
            }

            uint8_t *buf = packet->buffer();

            if (totalSize > packet->capacity())
                throw FactoryError("{PacketFactory::createSnapshotPacket} Snapshot too large");

            std::memcpy(buf, &header, sizeof(header));
            size_t offset = sizeof(header);

            for (const auto &[id, x, y, spriteId] : entities) {
                SnapshotEntityData packed{};
                packed.id = htonll(id);
                packed.x = htonf(x);
                packed.y = htonf(y);
                packed.spriteId = htonl(spriteId);

                std::memcpy(buf + offset, &packed, sizeof(packed));
                offset += sizeof(packed);
            }

            packet->setSize(totalSize);
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{PacketFactory::createSnapshotPacket} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IPacket> PacketFactory::createScorePacket(const sockaddr_in &addr, uint32_t score) const noexcept
    {
        ScoreData scoreData;
        scoreData.header = makeHeader(Protocol::UDP::SCORE, VERSION, sizeof(ScoreData));
        scoreData.score = htonl(score);
        try {
            auto packet = makePacket<ScoreData>(addr, scoreData);
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{PacketFactory::createScorePacket} " << e.what() << std::endl;
            return nullptr;
        }
    }

} // namespace Net::Factory
