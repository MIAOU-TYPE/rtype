/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

#pragma once

template<typename Type>
std::shared_ptr<Net::IServerPacket> Net::Factory::PacketFactory::makePacket(const sockaddr_in &addr, const Type &packetData) const noexcept
{
    auto buffer = _packet->clone();
    buffer->setAddress(addr);
    std::memcpy(buffer->buffer(), &packetData, sizeof(Type));
    buffer->setSize(sizeof(Type));
    return buffer;
}
