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

    HeaderData PacketFactory::makeHeader(uint8_t type, uint8_t version, uint16_t size) noexcept
    {
        HeaderData header;

        header.type = type;
        header.version = version;
        header.size = htons(size);
        return header;
    }

    std::shared_ptr<IPacket> PacketFactory::makeDefault(const sockaddr_in &addr, uint8_t flag) const noexcept
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

    std::shared_ptr<IPacket> PacketFactory::makeEntityCreate(
        const sockaddr_in &addr, size_t id, float x, float y, uint16_t sprite) const noexcept
    {
        EntityCreateData entityCreatePacket;
        entityCreatePacket.header = makeHeader(Protocol::ENTITY_CREATE, VERSION, sizeof(EntityCreateData));
        entityCreatePacket.id = htobe64(id);
        entityCreatePacket.x = htonf(x);
        entityCreatePacket.y = htonf(y);
        entityCreatePacket.sprite = htons(sprite);

        try {
            auto packet = makePacket<EntityCreateData>(addr, entityCreatePacket);
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{PacketFactory::makeEntityCreate} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IPacket> PacketFactory::makeEntityDestroy(const sockaddr_in &addr, size_t id) const noexcept
    {
        EntityDestroyData entityDestroyPacket;
        entityDestroyPacket.header = makeHeader(Protocol::ENTITY_DESTROY, VERSION, sizeof(EntityDestroyData));
        entityDestroyPacket.id = htobe64(id);

        try {
            auto packet = makePacket<EntityDestroyData>(addr, entityDestroyPacket);
            return packet;
        } catch (const FactoryError &e) {
            std::cerr << "{PacketFactory::makeEntityDestroy} " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<IPacket> PacketFactory::makeDamage(
        const sockaddr_in &addr, uint32_t id, uint16_t amount) const noexcept
    {
        DamageData damageData;
        damageData.header = makeHeader(Protocol::DAMAGE_EVENT, VERSION, sizeof(DamageData));
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
} // namespace Net::Factory
