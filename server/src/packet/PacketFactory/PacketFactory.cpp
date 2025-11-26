/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#include "PacketFactory.hpp"

// static uint32_t htonf(float f)
// {
//     uint32_t tmp;
//     std::memcpy(&tmp, &f, sizeof(f));
//     tmp = htonl(tmp);
//     return tmp;
// }

// static float ntohf(uint32_t v)
// {
//     v = ntohl(v);
//     float f;
//     std::memcpy(&f, &v, sizeof(f));
//     return f;
// }

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

    std::shared_ptr<IServerPacket> PacketFactory::makeDefault(const sockaddr_in &addr, uint8_t flag) noexcept
    {
        DefaultPacket defaultPacket;
        defaultPacket.header = makeHeader(flag, flag, sizeof(DefaultPacket));

        auto buffer = _packet->clone();
        buffer->setAddress(addr);
        std::memcpy(buffer->buffer(), &defaultPacket, sizeof(DefaultPacket));
        buffer->setSize(sizeof(DefaultPacket));
        return buffer;
    }

    //==============
    // TODO
    //==============
    std::shared_ptr<IServerPacket> PacketFactory::makeEntityCreate(
        const sockaddr_in &addr, size_t id, float x, float y, uint16_t sprite) noexcept
    {
        EntityCreatePacket entityCreatePacket;
        entityCreatePacket.header = makeHeader(ENTITY_CREATE, ENTITY_CREATE, sizeof(EntityCreatePacket));
        entityCreatePacket.id = htobe64(id);
        entityCreatePacket.x = htonf(x);
        entityCreatePacket.y = htonf(y);
        entityCreatePacket.sprite = htons(sprite);

        auto buffer = _packet->clone();
        buffer->setAddress(addr);
        std::memcpy(buffer->buffer(), &entityCreatePacket, sizeof(EntityCreatePacket));
        buffer->setSize(sizeof(EntityCreatePacket));
        return buffer;
    }

    //==============
    // TODO
    //==============
    std::shared_ptr<IServerPacket> PacketFactory::makeEntityDestroy(const sockaddr_in &addr, size_t id) noexcept
    {
        (void) id;

        auto buffer = _packet->clone();
        buffer->setAddress(addr);
        return buffer;
    }

    //==============
    // TODO
    //==============
    std::shared_ptr<IServerPacket> PacketFactory::makeDamage(
        const sockaddr_in &addr, size_t id, uint16_t amount) noexcept
    {
        (void) id;
        (void) amount;

        auto buffer = _packet->clone();
        buffer->setAddress(addr);
        return buffer;
    }
} // namespace Net::Factory
