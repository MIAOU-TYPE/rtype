/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ISessionManager
*/

#pragma once
#include <string>

#ifndef _WIN32
    #include <arpa/inet.h>
    #include <netinet/in.h>
#else
    #include <winsock2.h>
#endif

/**
 * @namespace Net::Server
 * @brief Namespace for server-side networking components.
 */
namespace Net::Server
{
    /**
     * @class ISessionManager
     * @brief Interface for managing network sessions.
     */
    class ISessionManager {
      public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~ISessionManager() = default;

        /**
         * @brief Get or create a session for the given address.
         * @param addr The address of the client.
         * @return The session ID.
         */
        virtual int getOrCreateSession(const sockaddr_in &addr) = 0;

        /**
         * @brief Get the session ID for the given address.
         * @param addr The address of the client.
         * @return The session ID, or -1 if not found.
         */
        virtual int getSessionId(const sockaddr_in &addr) const = 0;

        /**
         * @brief Remove a session by its ID.
         * @param sessionId The ID of the session to remove.
         */
        virtual void removeSession(int sessionId) = 0;

        /**
         * @brief Get the address associated with a session ID.
         * @param sessionId The ID of the session.
         * @return Pointer to the sockaddr_in structure, or nullptr if not found.
         */
        virtual const sockaddr_in *getAddress(int sessionId) const = 0;
    };
} // namespace Net::Server