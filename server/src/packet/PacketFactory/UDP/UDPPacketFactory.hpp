/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPPacketFactory
*/

#pragma once
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <lz4.h>
#include <map>
#include <memory>
#include <utility>
#include <vector>
#include "DamageData.hpp"
#include "DefaultData.hpp"
#include "DestroyData.hpp"
#include "Endian.hpp"
#include "IPacket.hpp"
#include "InputData.hpp"
#include "ScoreData.hpp"
#include "SnapEntityData.hpp"
#include "UDPTypesData.hpp"

/**
 * @namespace Net
 * @brief Namespace for networking-related classes and functions.
 */
/**
 * @namespace Factory
 * @brief Namespace for factory classes related to networking.
 */
namespace Net::Factory
{
    class FactoryError : public std::exception {
      public:
        /**
         * @brief Constructs a new FactoryError object with the specified message.
         * @param message The error message.
         */
        explicit FactoryError(std::string message) : _message(std::move(message))
        {
        }

        /**
         * @brief Returns the error message.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message = ""; ///> The error message.
    };

    /**
     * @class UDPPacketFactory
     * @brief Factory class for creating various types of network packets.
     * @details This class provides methods to create different types of packets
     * such as connect/disconnect, input, default, entity creation/destruction, and damage events.
     */
    class UDPPacketFactory {
      public:
        /**
         * @brief Constructs a new UDPPacketFactory object.
         * @param packet A shared pointer to an IPacket used as a template for creating packets.
         */
        explicit UDPPacketFactory(const std::shared_ptr<Net::IPacket> &packet);

        /**
         * @brief Destructor for UDPPacketFactory.
         */
        ~UDPPacketFactory() = default;

        /**
         * @brief Creates a default packet with the specified flag.
         * @param flag The flag to set in the default packet.
         * @param addr The address to which the packet will be sent.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> makeDefault(const sockaddr_in &addr, uint8_t flag) const noexcept;

        /**
         * @brief Creates a DamageData for the specified entity ID and damage amount.
         * @param addr The address to which the packet will be sent.
         * @param id The ID of the entity receiving damage.
         * @param amount The amount of damage to apply.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> makeDamage(
            const sockaddr_in &addr, uint32_t id, uint16_t amount) const noexcept;

        /**
         * @brief Creates snapshot packets from the given entities, server tick, and maximum packet size.
         * @param entities The list of snapshot entities to include in the packets.
         * @param serverTick The current server tick.
         * @param maxPacketBytes The maximum size of each packet in bytes.
         * @return A vector of shared pointers to the created IPackets.
         */
        [[nodiscard]] std::vector<std::shared_ptr<IPacket>> createSnapshotPackets(
            const std::vector<SnapshotEntity> &entities, uint32_t serverTick, size_t maxPacketBytes) const noexcept;

        /**
         * @brief Creates a score packet with the specified address and score.
         * @param addr The address to which the packet will be sent.
         * @param score The score to include in the packet.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> createScorePacket(
            const sockaddr_in &addr, uint32_t score) const noexcept;

        /**
         * @brief Creates a destroy entity packet with the specified address and entity ID.
         * @param entityId The ID of the entity to be destroyed.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> createDestroyEntityPacket(size_t entityId) const noexcept;

      private:
        /*
        * @struct BuiltPkt
        * @brief Represents a built packet with its properties.
        */
        struct BuiltPkt {
            std::shared_ptr<IPacket> pkt; ///> Pointer to the built IPacket.
            bool compressed; ///> Indicates if the packet is compressed.
            uint16_t count; ///> Number of entities in the packet.
        };

        /**
         * @struct ChunkSizes
         * @brief Represents the sizes of chunks for packet creation.
         */
        struct ChunkSizes {
            size_t maxPacketBytes; ///> Maximum size of the packet in bytes.
            size_t maxCompBytes; ///> Maximum size of the compressed data in bytes.
            size_t maxEntitiesPerPkt; ///> Maximum number of entities per packet.
        };

        /**
         * @brief Computes the chunk sizes based on the maximum packet size.
         * @param maxPacketBytes The maximum size of the packet in bytes.
         * @return An optional ChunkSizes structure if computation is successful; std::nullopt otherwise.
         */
        [[nodiscard]] static std::optional<ChunkSizes> computeChunkSizes(size_t maxPacketBytes) noexcept;

        /**
         * @brief Packs the given entities into a raw buffer.
         * @param rawBuf The buffer to store the packed entities.
         * @param entities The list of snapshot entities to pack.
         * @param cursorEntity The starting index of entities to pack.
         * @param count The number of entities to pack.
         */
        static void packEntitiesToRaw(
            std::vector<char> &rawBuf, const std::vector<SnapshotEntity> &entities, size_t cursorEntity, size_t count);

        /**
         * @brief Attempts to compress the raw buffer into the compressed buffer.
         * @param compBuf The buffer to store the compressed data.
         * @param rawBuf The raw buffer containing uncompressed data.
         * @param rawSize The size of the raw buffer.
         * @param wireUnc The size of the uncompressed data on the wire.
         * @param sz The chunk sizes for packet creation.
         * @param outCompSize The size of the compressed data if compression is successful.
         * @return True if compression was successful and beneficial; false otherwise.
         */
        [[nodiscard]] bool tryCompress(std::vector<char> &compBuf, const std::vector<char> &rawBuf, size_t rawSize,
            size_t wireUnc, const ChunkSizes &sz, size_t &outCompSize) const;

        /**
         * @brief Builds a raw packet with the specified parameters.
         * @param rawBuf The buffer containing raw data.
         * @param rawSize The size of the raw data.
         * @param count The number of entities in the packet.
         * @param serverTick The current server tick.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> buildRawPacket(
            const std::vector<char> &rawBuf, size_t rawSize, uint16_t count, uint32_t serverTick) const;

        /**
         * @brief Builds a compressed packet with the specified parameters.
         * @param compBuf The buffer containing compressed data.
         * @param rawSize The size of the uncompressed data.
         * @param compSize The size of the compressed data.
         * @param count The number of entities in the packet.
         * @param serverTick The current server tick.
         * @return A shared pointer to the created IPacket.
         */
        [[nodiscard]] std::shared_ptr<IPacket> buildCompressedPacket(const std::vector<char> &compBuf, size_t rawSize,
            size_t compSize, uint16_t count, uint32_t serverTick) const;

        /**
         * @brief Patches the chunk information in the packet header.
         * @param bp The built packet containing the IPacket.
         * @param chunkIndexNet The network byte order chunk index.
         * @param chunkCountNet The network byte order chunk count.
         */
        static void patchChunkInfo(const BuiltPkt &bp, uint16_t chunkIndexNet, uint16_t chunkCountNet) noexcept;

        /**
         * @brief Builds one chunk of snapshot entities into a packet.
         * @param entities The list of snapshot entities to include in the chunk.
         * @param cursorEntity The current index of the entity to process.
         * @param sz The chunk sizes for packet creation.
         * @param serverTick The current server tick.
         * @param rawBuf The buffer to use for raw data packing.
         * @param compBuf The buffer to use for compressed data packing.
         * @return An optional BuiltPkt containing the created packet and its properties; std::nullopt on failure.
         */
        [[nodiscard]] std::optional<BuiltPkt> buildOneChunk(const std::vector<SnapshotEntity> &entities,
            size_t &cursorEntity, const ChunkSizes &sz, uint32_t serverTick, std::vector<char> &rawBuf,
            std::vector<char> &compBuf) const;

        /**
         * @brief Creates a HeaderData with the specified parameters.
         * @param type The type of the packet.
         * @param version The version of the packet.
         * @param size The size of the packet.
         * @return The constructed HeaderData.
         */
        [[nodiscard]] static HeaderData makeHeader(uint8_t type, uint8_t version, uint16_t size) noexcept;

        /**
         * @brief Creates a packet of the specified type with the given address and packet data.
         * @tparam Type The type of the packet data. (struct representing the packet)
         * @param addr The address to which the packet will be sent.
         * @param packetData The data to be included in the packet.
         * @return A shared pointer to the created IPacket.
         */
        template <typename Type>
        [[nodiscard]] std::shared_ptr<IPacket> makePacket(const sockaddr_in &addr, const Type &packetData) const;

        std::shared_ptr<IPacket> _packet = nullptr; ///> Pointer to the template IPacket used for creating packets.

        static constexpr uint8_t VERSION = 1; ///> The version of the packet factory.
    };
} // namespace Net::Factory

#include "UDPPacketFactory.tpp"
