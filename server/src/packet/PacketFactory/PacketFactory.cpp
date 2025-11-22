/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#include "PacketFactory.hpp"

using namespace Net;
using namespace Factory;

PacketFactory::PacketFactory(std::shared_ptr<IServerPacket> packet) : _packet(packet)
{
}

const HeaderPacket &PacketFactory::makeHeader(uint8_t type, uint8_t version, uint16_t size) noexcept
{
    HeaderPacket header;

    header.type = type;
    header.version = version;
    header.size = size;
    return header;
}

std::shared_ptr<IServerPacket> PacketFactory::makeConnectDisconnect(
    uint32_t clientId, uint8_t connectDisconnect) noexcept
{
    ConnectPacket connectPacket;
    connectPacket.clientId = htonl(clientId);
    connectPacket.header = makeHeader(connectDisconnect, connectDisconnect, sizeof(HeaderPacket) + sizeof(clientId));

    auto buffer = _packet->clone();
    std::memcpy(buffer->buffer(), &connectPacket, sizeof(ConnectPacket));
    buffer->setSize(sizeof(ConnectPacket));
    return buffer;
}

std::shared_ptr<IServerPacket> PacketFactory::makeInput(uint32_t id, float dx, float dy, bool shooting) noexcept
{
    (void) id;
    (void) dx;
    (void) dy;
    (void) shooting;
    return _packet->clone();
}

std::shared_ptr<IServerPacket> PacketFactory::makeDefault(uint8_t flag) noexcept
{
    DefaultPacket defaultPacket;
    defaultPacket.header = makeHeader(flag, flag, sizeof(HeaderPacket));

    auto buffer = _packet->clone();
    std::memcpy(buffer->buffer(), &defaultPacket, sizeof(DefaultPacket));
    buffer->setSize(sizeof(DefaultPacket));
    return buffer;
}

std::shared_ptr<IServerPacket> PacketFactory::makeEntityCreate(uint32_t id, float x, float y, uint16_t sprite) noexcept
{
    (void) id;
    (void) x;
    (void) y;
    (void) sprite;
    return _packet->clone();
}

std::shared_ptr<IServerPacket> PacketFactory::makeEntityDestroy(uint32_t id) noexcept
{
    (void) id;
    return _packet->clone();
}

std::shared_ptr<IServerPacket> PacketFactory::makeDamage(uint32_t id, uint16_t amount) noexcept
{
    (void) id;
    (void) amount;
    return _packet->clone();
}
