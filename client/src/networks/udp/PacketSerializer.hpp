/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketSerializer
*/

#pragma once

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
#endif
#include <cstdint>
#include <cstring>
#include <exception>
#include <vector>
#include "PacketSnapshot.hpp"
#include "PacketTypes.hpp"

namespace Network
{
    /**
     * @class SerializerError
     * @brief Exception class for serialization errors.
     */
    class SerializerError : public std::exception {
      public:
        /**
         * @brief Constructor with error message.
         * @param message The error message.
         */
        explicit SerializerError(const char *message) : msg_(message)
        {
        }

        /**
         * @brief Get the error message.
         * @return The error message as a C-style string.
         */
        virtual const char *what() const noexcept override
        {
            return msg_;
        }

      private:
        const char *msg_; ///> The error message
    };

    /**
     * @brief Serializes network packets for client-to-server communication
     *
     * This class handles the serialization of various packet types into
     * byte buffers ready for network transmission. It ensures proper byte
     * ordering (network byte order) for multi-byte fields.
     */
    class PacketSerializer {
      public:
        /**
         * @brief Construct a new PacketSerializer object
         */
        PacketSerializer() = default;

        /**
         * @brief Destroy the PacketSerializer object
         */
        ~PacketSerializer() = default;

        /**
         * @brief Serialize a packet of type T into a byte buffer
         *
         * @tparam T The packet structure type to serialize
         * @param packet The packet instance to serialize
         * @return std::vector<uint8_t> The serialized byte buffer
         */
        template <typename T>
        std::vector<uint8_t> serialize(const T &packet);

        /**
         * @brief Serialize a PacketSnapshot with variable-length entity array
         *
         * @param packet The PacketSnapshot instance to serialize
         * @param entityCount The number of entities in the snapshot
         * @return std::vector<uint8_t> The serialized byte buffer
         */
        std::vector<uint8_t> serialize(const PacketSnapshot &packet, uint16_t entityCount);
    };

} // namespace Network

#include "PacketSerializer.tpp"