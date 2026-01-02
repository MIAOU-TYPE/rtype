/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPayload
*/

#pragma once
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
#include <string_view>

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <arpa/inet.h>
#endif
#include "reader/TCPReader.hpp"
#include "writer/TCPWriter.hpp"

namespace Net::TCP
{
    /**
     * @brief TCP Payload Header structure
     * Defines the header for TCP payloads, containing the type and request ID.
     */
    struct Header {
        uint8_t type;       ///> Payload type
        uint32_t requestId; ///> Request identifier
    };

    /**
     * @brief Build a TCP payload with header and body
     * @param type Payload type
     * @param requestId Request identifier
     * @param body Payload body as a vector of bytes
     * @return Serialized payload as a vector of bytes
     */
    std::vector<uint8_t> buildPayload(uint8_t type, uint32_t requestId, const std::vector<uint8_t> &body);

    /**
     * @brief Parse the TCP payload header from a byte buffer
     * @param payload Pointer to the byte buffer
     * @param n Size of the byte buffer
     * @return Parsed Header structure
     */
    Header parseHeader(const uint8_t *payload, size_t n);

    /**
     * @brief Create a Reader for the body of the TCP payload
     * @param payload Pointer to the byte buffer
     * @param n Size of the byte buffer
     * @return Reader object for reading the payload body
     */
    Reader bodyReader(const uint8_t *payload, size_t n);
} // namespace Net::TCP
