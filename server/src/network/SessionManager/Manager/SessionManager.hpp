/*
** EPITECH PROJECT, 2025
** RType
** File description:
** SessionManager
*/

#pragma once

#include <cstdint>
#include <mutex>
#include "ISessionManager.hpp"

#include <unordered_map>

/**
 * @brief A key representing a network address (IP and port).
 */
namespace Net::Server
{
    struct AddressKey {
        uint32_t ip;   ///> The IP address in network byte order.
        uint16_t port; ///> The port number in network byte order.

        /**
         * @brief Equality operator for AddressKey.
         */
        bool operator==(const AddressKey &other) const noexcept;
    };

    /**
     * @brief Hash function for AddressKey to be used in unordered_map.
     */
    struct AddressKeyHash {
        /**
         * @brief Hash function implementation.
         * @param k The AddressKey to hash.
         * @return The computed hash value.
         */
        std::size_t operator()(const AddressKey &k) const noexcept;
    };

    /**
     * @brief Manages network sessions by mapping addresses to session IDs.
     * Provides thread-safe operations to create, retrieve, and remove sessions.
     */
    class SessionManager : public ISessionManager {
      public:
        /**
         * @brief Get an existing session ID for the given address or create a new one.
         * @param addr The network address.
         * @return The session ID associated with the address.
         */
        int getOrCreateSession(const AddressIn &addr) override;

        /**
         * @brief Get the session ID for the given address.
         * @param addr The network address.
         * @return The session ID if it exists, otherwise -1.
         */
        int getSessionId(const AddressIn &addr) const override;

        /**
         * @brief Remove the session associated with the given session ID.
         * @param sessionId The session ID to remove.
         */
        void removeSession(int sessionId) override;

        /**
         * @brief Get the address associated with the given session ID.
         * @param sessionId The session ID.
         * @return A pointer to the AddressIn if found, otherwise nullptr.
         */
        const AddressIn *getAddress(int sessionId) const override;

      private:
        mutable std::mutex _mutex = {}; ///> Mutex for thread-safe access.

        std::unordered_map<AddressKey, int, AddressKeyHash> _addressToId = {}; ///> Map from AddressKey to session ID.
        std::unordered_map<int, AddressIn> _idToAddress = {};                  ///> Map from session ID to AddressIn.

        int _nextId = 1; ///> Next available session ID.
    };
} // namespace Net::Server