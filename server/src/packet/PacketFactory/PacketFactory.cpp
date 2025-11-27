/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#include "PacketFactory.hpp"

namespace Net::Factory
{

    PacketFactory::PacketFactory(const std::shared_ptr<IServerPacket> &packet)
    {
        if (!packet)
            throw FactoryError("{Net::Factory::PacketFactory::PacketFactory} Invalid IServerPacket pointer");
        _packet = packet;
    }

    HeaderPacket PacketFactory::makeHeader(uint8_t type, uint8_t version, uint16_t size) noexcept
    {
        HeaderPacket header;

        header.type = type;
        header.version = version;
        header.size = htons(size);
        return header;
    }

    std::shared_ptr<IServerPacket> PacketFactory::makeDefault(const sockaddr_in &addr, uint8_t flag) const noexcept
    {
        DefaultPacket defaultPacket;
        defaultPacket.header = makeHeader(flag, VERSION, sizeof(DefaultPacket));

        try {
            auto packet = makePacket<DefaultPacket>(addr, defaultPacket);
            return packet;
        } catch (const FactoryError &e) {
            return nullptr;
        }
    }

    std::shared_ptr<IServerPacket> PacketFactory::makeEntityCreate(
        const sockaddr_in &addr, size_t id, float x, float y, uint16_t sprite) const noexcept
    {
        EntityCreatePacket entityCreatePacket;
        entityCreatePacket.header = makeHeader(ENTITY_CREATE, VERSION, sizeof(EntityCreatePacket));
        entityCreatePacket.id = htobe64(id);
        entityCreatePacket.x = htonf(x);
        entityCreatePacket.y = htonf(y);
        entityCreatePacket.sprite = htons(sprite);

        try {
            auto packet = makePacket<EntityCreatePacket>(addr, entityCreatePacket);
            return packet;
        } catch (const FactoryError &e) {
            return nullptr;
        }
    }

    std::shared_ptr<IServerPacket> PacketFactory::makeEntityDestroy(const sockaddr_in &addr, size_t id) const noexcept
    {
        EntityDestroyPacket entityDestroyPacket;
        entityDestroyPacket.header = makeHeader(ENTITY_DESTROY, VERSION, sizeof(EntityDestroyPacket));
        entityDestroyPacket.id = htobe64(id);

        try {
            auto packet = makePacket<EntityDestroyPacket>(addr, entityDestroyPacket);
            return packet;
        } catch (const FactoryError &e) {
            return nullptr;
        }
    }

    std::shared_ptr<IServerPacket> PacketFactory::makeDamage(
        const sockaddr_in &addr, uint32_t id, uint16_t amount) const noexcept
    {
        DamagePacket damagePacket;
        damagePacket.header = makeHeader(DAMAGE_EVENT, VERSION, sizeof(DamagePacket));
        damagePacket.id = htonl(id);
        damagePacket.amount = htons(amount);

        try {
            auto packet = makePacket<DamagePacket>(addr, damagePacket);
            return packet;
        } catch (const FactoryError &e) {
            return nullptr;
        }
    }
} // namespace Net::Factory
