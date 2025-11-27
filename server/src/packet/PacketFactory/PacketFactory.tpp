/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketFactory
*/

template<typename Type>
std::shared_ptr<Net::IServerPacket> Net::Factory::PacketFactory::makePacket(const sockaddr_in &addr, const Type &packetData) const noexcept
{
    auto buffer = _packet->clone();
    if (sizeof(Type) > buffer->capacity())
    buffer->setAddress(addr);
    std::memcpy(buffer->buffer(), &packetData, sizeof(Type));
    buffer->setSize(sizeof(Type));
    return buffer;
}
