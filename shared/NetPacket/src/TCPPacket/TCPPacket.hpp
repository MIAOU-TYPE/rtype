/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacket
*/

#pragma once

#include <cstring>
#include <vector>
#include "IPacket.hpp"

namespace Net
{
    /**
     * @class TCPPacket
     * @brief Implementation of the IPacket interface for TCP packets.
     * @details This class provides a concrete implementation of the IPacket interface,
     * allowing for the creation and manipulation of TCP packets, including buffer management
     * and source address handling.
     */
    class TCPPacket final : public IPacket {
      public:
        /**
         * @brief Construct a new TCPPacket object with a specified capacity.
         * @param capacity The capacity of the packet buffer.
         */
        explicit TCPPacket(size_t capacity = 64 * 1024);

        /**
         * @brief Retrieves the packet buffer.
         * @return A pointer to the packet buffer.
         */
        [[nodiscard]] uint8_t *buffer() override;

        /**
         * @brief Retrieves the packet buffer (const version).
         * @return A const pointer to the packet buffer.
         */
        [[nodiscard]] const uint8_t *buffer() const override;

        /**
         * @brief Retrieves the size of the packet.
         * @return The size of the packet.
         */
        [[nodiscard]] size_t size() const override;

        /**
         * @brief Sets the size of the packet.
         * @param s The size to set for the packet.
         */
        void setSize(size_t s) override;

        /**
         * @brief Retrieves the source address of the packet.
         * @return A pointer to the sockaddr_in structure representing the source address.
         */
        [[nodiscard]] const sockaddr_in *address() const override;

        /**
         * @brief Sets the source address of the packet.
         * @param addr The sockaddr_in structure representing the source address to set.
         */
        void setAddress(const sockaddr_in &addr) override;

        /**
         * @brief Creates a clone of the current TCPPacket.
         * @return A shared pointer to the cloned IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> clone() const override;

        /**
         * @brief Creates a new instance of the TCPPacket.
         * @return A shared pointer to the newly created IPacket instance.
         */
        [[nodiscard]] std::shared_ptr<IPacket> newPacket() const override;

        /**
         * @brief Retrieves the capacity of the packet buffer.
         * @return The capacity of the packet buffer.
         */
        [[nodiscard]] size_t capacity() const noexcept override;

      private:
        sockaddr_in _addr{};       ///> Source address of the packet
        std::vector<uint8_t> _buf; ///> Packet buffer
        size_t _size = 0;          ///> Size of the packet
    };
} // namespace Net
