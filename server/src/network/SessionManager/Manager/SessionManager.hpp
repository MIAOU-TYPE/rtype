/*
** EPITECH PROJECT, 2025
** RType
** File description:
** SessionManager
*/

#pragma once

#include <cstdint>
#include "ISessionManager.hpp"
#include <shared_mutex>
#include <mutex>

#include <unordered_map>
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netinet/in.h>
#endif

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
         * @param address The network address.
         * @return The session ID associated with the address.
         */
        int getOrCreateSession(const sockaddr_in &address) override;

        /**
         * @brief Get the session ID for the given address.
         * @param address The network address.
         * @return The session ID if it exists, otherwise -1.
         */
        int getSessionId(const sockaddr_in &address) const override;

        /**
         * @brief Remove the session associated with the given session ID.
         * @param sessionId The session ID to remove.
         */
        void removeSession(int sessionId) override;

        /**
         * @brief Get the address associated with the given session ID.
         * @param sessionId The session ID.
         * @return A pointer to the sockaddr_in if found, otherwise nullptr.
         */
        const sockaddr_in *getAddress(int sessionId) const override;

        /**
         * @brief Get all active sessions.
         * @return A vector of pairs containing session IDs and their corresponding addresses.
         */
        std::vector<std::pair<int, sockaddr_in>> getAllSessions() const override;

        /**
         * @brief Apply a function to each session.
         * @param func The function to apply, taking session ID and address as parameters.
         */
        void forEachSession(const std::function<void(int, const sockaddr_in &)> &func) const override;

      private:
        mutable std::shared_mutex _mutex = {}; ///> Mutex for thread-safe access.

        std::unordered_map<AddressKey, int, AddressKeyHash> _addressToId = {}; ///> Map from AddressKey to session ID.
        std::unordered_map<int, sockaddr_in> _idToAddress = {};                ///> Map from session ID to sockaddr_in.

        int _nextId = 1; ///> Next available session ID.
    };
} // namespace Net::Server
