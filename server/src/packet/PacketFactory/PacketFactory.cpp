/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#include "PacketFactory.hpp"

namespace Net::Factory
{

    PacketFactory::PacketFactory(const std::shared_ptr<IServerPacket> &packet) : _packet(packet)
    {
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
        defaultPacket.header = makeHeader(flag, flag, sizeof(DefaultPacket));

        return makePacket<DefaultPacket>(addr, defaultPacket);
    }

    std::shared_ptr<IServerPacket> PacketFactory::makeEntityCreate(
        const sockaddr_in &addr, size_t id, float x, float y, uint16_t sprite) const noexcept
    {
        EntityCreatePacket entityCreatePacket;
        entityCreatePacket.header = makeHeader(ENTITY_CREATE, ENTITY_CREATE, sizeof(EntityCreatePacket));
        entityCreatePacket.id = htobe64(id);
        entityCreatePacket.x = htonf(x);
        entityCreatePacket.y = htonf(y);
        entityCreatePacket.sprite = htons(sprite);

        return makePacket<EntityCreatePacket>(addr, entityCreatePacket);
    }

    std::shared_ptr<IServerPacket> PacketFactory::makeEntityDestroy(const sockaddr_in &addr, size_t id) const noexcept
    {
        EntityDestroyPacket entityDestroyPacket;
        entityDestroyPacket.header = makeHeader(ENTITY_DESTROY, ENTITY_DESTROY, sizeof(EntityDestroyPacket));
        entityDestroyPacket.id = htobe64(id);

        return makePacket<EntityDestroyPacket>(addr, entityDestroyPacket);
    }

    std::shared_ptr<IServerPacket> PacketFactory::makeDamage(
        const sockaddr_in &addr, uint32_t id, uint16_t amount) const noexcept
    {
        DamagePacket damagePacket;
        damagePacket.header = makeHeader(DAMAGE_EVENT, DAMAGE_EVENT, sizeof(DamagePacket));
        damagePacket.id = htonl(id);
        damagePacket.amount = htons(amount);

        return makePacket<DamagePacket>(addr, damagePacket);
    }
} // namespace Net::Factory
