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

PacketFactory::~PacketFactory()
{
    if (_packet)
        _packet = nullptr;
}

HeaderPacket PacketFactory::makeHeader(uint8_t type, uint8_t version, uint16_t size) noexcept
{
    HeaderPacket header;

    header.type = type;
    header.version = version;
    header.size = size;
    return header;
}

std::shared_ptr<IServerPacket> PacketFactory::makeConnectDisconnect(
    const sockaddr_in &addr, uint32_t clientId, uint8_t connectDisconnect) noexcept
{
    ConnectPacket connectPacket;
    connectPacket.clientId = htonl(clientId);
    connectPacket.header = makeHeader(connectDisconnect, connectDisconnect, sizeof(ConnectPacket));

    auto buffer = _packet->clone();
    buffer->setAddress(addr);
    std::memcpy(buffer->buffer(), &connectPacket, sizeof(ConnectPacket));
    buffer->setSize(sizeof(ConnectPacket));
    return buffer;
}

//==============
// TODO
//==============
std::shared_ptr<IServerPacket> PacketFactory::makeInput(
    const sockaddr_in &addr, uint32_t id, float dx, float dy, bool shooting) noexcept
{
    (void) id;
    (void) dx;
    (void) dy;
    (void) shooting;

    auto buffer = _packet->clone();
    buffer->setAddress(addr);
    return buffer;
}

std::shared_ptr<IServerPacket> PacketFactory::makeDefault(const sockaddr_in &addr, uint8_t flag) noexcept
{
    DefaultPacket defaultPacket;
    defaultPacket.header = makeHeader(flag, flag, sizeof(HeaderPacket));

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
    const sockaddr_in &addr, uint32_t id, float x, float y, uint16_t sprite) noexcept
{
    (void) id;
    (void) x;
    (void) y;
    (void) sprite;

    auto buffer = _packet->clone();
    buffer->setAddress(addr);
    return buffer;
}

//==============
// TODO
//==============
std::shared_ptr<IServerPacket> PacketFactory::makeEntityDestroy(const sockaddr_in &addr, uint32_t id) noexcept
{
    (void) id;

    auto buffer = _packet->clone();
    buffer->setAddress(addr);
    return buffer;
}

//==============
// TODO
//==============
std::shared_ptr<IServerPacket> PacketFactory::makeDamage(const sockaddr_in &addr, uint32_t id, uint16_t amount) noexcept
{
    (void) id;
    (void) amount;

    auto buffer = _packet->clone();
    buffer->setAddress(addr);
    return buffer;
}
