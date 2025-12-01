/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SnapshotParser
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
#include <stdexcept>
#include <vector>
#include "PacketSnapshot.hpp"
#include "PacketTypes.hpp"

namespace Network
{

    /**
     * @class ParserError
     * @brief Exception class for parsing errors.
     */
    class ParserError : public std::runtime_error {
      public:
        explicit ParserError(const std::string &message) : std::runtime_error("Parser error: " + message)
        {
        }
    };

    /**
     * @class SnapshotParser
     * @brief Parser for converting raw snapshot packets into ECS-usable structures.
     */
    class SnapshotParser {
      public:
        SnapshotParser() = default;
        ~SnapshotParser() = default;

        /**
         * @brief Parse a raw byte buffer into a PacketSnapshot structure
         *
         * @param buffer The raw byte buffer containing the snapshot packet
         * @param bufferSize The size of the buffer in bytes
         * @return PacketSnapshot The parsed snapshot structure
         * @throws ParserError if the buffer is invalid or malformed
         */
        PacketSnapshot parse(const uint8_t *buffer, size_t bufferSize);

        /**
         * @brief Parse a raw byte vector into a PacketSnapshot structure
         *
         * @param buffer The raw byte vector containing the snapshot packet
         * @return PacketSnapshot The parsed snapshot structure
         * @throws ParserError if the buffer is invalid or malformed
         */
        PacketSnapshot parse(const std::vector<uint8_t> &buffer);

      private:
        /**
         * @brief Parse the entity count from the buffer
         *
         * @param buffer Pointer to the entity count location in the buffer
         * @return uint16_t The entity count in host byte order
         */
        uint16_t parseEntityCount(const uint8_t *buffer);

        /**
         * @brief Parse a single entity from the buffer
         *
         * @param buffer Pointer to the entity data in the buffer
         * @return SnapshotEntity The parsed entity structure
         */
        SnapshotEntity parseEntity(const uint8_t *buffer);
    };

} // namespace Network