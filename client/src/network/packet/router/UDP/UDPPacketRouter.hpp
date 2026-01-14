/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPPacketRouter
*/

#pragma once

#include <cstring>
#include <iostream>
#include <memory>

#include <chrono>
#include <lz4.h>
#include <utility>
#include <vector>
#include "AcceptData.hpp"
#include "DefaultData.hpp"
#include "DestroyData.hpp"
#include "Endian.hpp"
#include "HeaderData.hpp"
#include "IClientMessageSink.hpp"
#include "IPacket.hpp"
#include "ScoreData.hpp"
#include "SnapEntityData.hpp"
#include "UDPTypesData.hpp"
#include <unordered_map>

namespace Ecs
{
    class PacketRouterError : public std::exception {
      public:
        /**
         * @brief Constructor for PacketRouterError.
         * @param message The error message to be associated with the exception.
         */
        explicit PacketRouterError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Retrieves the error message.
         * @return A C-style string representing the error message.
         */
        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    /**
     * @brief Routes incoming packets to appropriate handlers based on packet type.
     */
    class UDPPacketRouter {
      public:
        /**
         * @brief Constructs a UDPPacketRouter with the given IClientMessageSink.
         * @param sink Shared pointer to the IClientMessageSink for handling routed messages.
         */
        explicit UDPPacketRouter(const std::shared_ptr<IClientMessageSink> &sink);

        /**
         * @brief Handles an incoming packet by routing it to the appropriate handler.
         * @param packet Shared pointer to the incoming IPacket to be processed.
         */
        void handlePacket(const std::shared_ptr<Net::IPacket> &packet) const;

      private:
        /**
         * @brief Handler for SNAP_ENTITY packets with raw payload.
         * @param payload Pointer to the payload data of the SNAP_ENTITY packet.
         * @param size Size of the payload data.
         */
        void handleSnapEntityRaw(const uint8_t *payload, size_t size) const;

        /**
         * @brief Handler for SNAP_ENTITY packets with compressed payload.
         * @param payload Pointer to the payload data of the SNAP_ENTITY_COMPRESSED packet.
         * @param size Size of the payload data.
         */
        void handleSnapEntityCompressed(const uint8_t *payload, size_t size) const;

        /**
         * @brief Validates the header of an incoming packet.
         * @param packet The incoming IPacket to validate.
         * @param header The HeaderData extracted from the incoming packet.
         * @return True if the header is valid, false otherwise.
         */
        [[nodiscard]] static bool isHeaderValid(const Net::IPacket &packet, const HeaderData &header);

        /**
         * @brief Validates the incoming packet.
         * @param packet Shared pointer to the incoming IPacket to validate.
         * @return True if the packet is valid, false otherwise.
         */
        [[nodiscard]] static bool isPacketValid(const std::shared_ptr<Net::IPacket> &packet) noexcept;

        /**
         * @brief Handler for ACCEPT packets.
         */
        void handleAccept(const uint8_t *payload, size_t size) const;

        /**
         * @brief Handler for REJECT packets.
         */
        void handleReject() const;

        /**
         * @brief Handler for PONG packets.
         */
        void handlePong() const;

        /**
         * @brief Handler for GAME_OVER packets.
         */
        void handleGameOver() const;

        /**
         * @brief Handler for SCORE packets.
         * @param payload Pointer to the payload data of the SCORE packet.
         * @param size Size of the payload data.
         */
        void handleScore(const uint8_t *payload, size_t size) const;

        /**
         * @brief Handler for DESTROY packets.
         * @param payload Pointer to the payload data of the DESTROY packet.
         * @param size Size of the payload data.
         */
        void handleDestroy(const uint8_t *payload, size_t size) const;

        /**
         * @brief Extracts the header from the incoming packet.
         * @param packet The incoming IPacket to extract the header from.
         * @param outHeader Reference to the HeaderData to populate with extracted data
         * @return True if the header was successfully extracted and validated, false otherwise.
         */
        [[nodiscard]] static bool extractHeader(const Net::IPacket &packet, HeaderData &outHeader) noexcept;

        /**
         * @brief Dispatches the packet to the appropriate handler based on its type.
         * @param header The HeaderData of the incoming packet.
         * @param payload Pointer to the payload data of the packet
         * @param payloadSize Size of the payload data.
         */
        void dispatchPacket(const HeaderData &header, const uint8_t *payload, std::size_t payloadSize) const;

        static constexpr uint8_t PROTOCOL_VERSION = 1; ///> Expected protocol version for incoming packets.

        std::shared_ptr<IClientMessageSink> _sink; ///> Pointer to the IClientMessageSink for handling routed messages.

        /**
         * @brief Structure representing a pending snapshot being assembled from multiple chunks.
         */
        struct PendingSnapshot {
            uint16_t chunkCount = 0;                  ///> Chunks for the snapshot
            std::vector<uint8_t> received;            ///> Bitmap of received chunks
            std::vector<SnapshotEntity> merged;       ///> Merged entities from received chunks
            std::chrono::steady_clock::time_point t0; ///> Timestamp of the first received chunk
        };

        mutable std::unordered_map<uint32_t, PendingSnapshot> _pending; ///> Snapshots en attente d'assemblage

        /**
         * @brief Time-to-live duration for pending snapshots before they are purged.
         */
        static constexpr auto PendingTTL = std::chrono::milliseconds(400);

        /**
         * @brief Purges expired pending snapshots based on the current time.
         * @param now The current time point used to determine expiration.
         */
        void purgeExpired(std::chrono::steady_clock::time_point now) const;

        /**
         * @brief Retrieves or resets the accumulator for a given server tick and chunk count.
         * @param serverTick The server tick associated with the snapshot.
         * @param chunkCount The total number of chunks expected for the snapshot.
         * @param now The current time point used for timestamping.
         * @return Reference to the PendingSnapshot accumulator.
         */
        [[nodiscard]] PendingSnapshot &getOrResetAcc(
            uint32_t serverTick, uint16_t chunkCount, std::chrono::steady_clock::time_point now) const;

        /**
         * @brief Accepts a chunk for the given PendingSnapshot accumulator.
         * @param acc Reference to the PendingSnapshot accumulator.
         * @param chunkIndex The index of the chunk being accepted.
         * @return True if the chunk was accepted, false if it was already received or invalid.
         */
        [[nodiscard]] static bool acceptChunk(PendingSnapshot &acc, uint16_t chunkIndex);

        /**
         * @brief Checks if the PendingSnapshot accumulator has received all expected chunks.
         * @param acc The PendingSnapshot accumulator to check.
         * @return True if all chunks have been received, false otherwise.
         */
        [[nodiscard]] static bool isComplete(const PendingSnapshot &acc);

        /**
         * @brief Appends entities from raw data to the PendingSnapshot accumulator.
         * @param acc Reference to the PendingSnapshot accumulator.
         * @param raw Pointer to the raw entity data.
         * @param count The number of entities to append.
         */
        static void appendEntitiesFromRaw(PendingSnapshot &acc, const uint8_t *raw, uint16_t count);

        /**
         * @brief Decompresses a compressed snapshot payload into raw entity data.
         * @param payload Pointer to the compressed payload data.
         * @param size Size of the compressed payload data.
         * @param rawSize Expected size of the decompressed raw data.
         * @param compSize Size of the compressed data.
         * @param outRaw Reference to a vector to store the decompressed raw data.
         * @return True if decompression was successful, false otherwise.
         */
        [[nodiscard]] static bool decompressSnapshotPayload(
            const uint8_t *payload, size_t size, uint16_t rawSize, uint16_t compSize, std::vector<char> &outRaw);
    };
} // namespace Ecs
