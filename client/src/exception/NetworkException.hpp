/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkException
*/

#pragma once

#include <exception>
#include <string>

namespace Client::Exception
{

    /**
     * @brief Base exception class for network operations
     *
     * This class provides a comprehensive exception handling mechanism
     * for all network-related errors in the R-Type project.
     */
    class NetworkException : public std::exception {
      public:
        /**
         * @brief Construct a new Network Exception object
         *
         * @param message The error message describing the exception
         */
        explicit NetworkException(const std::string &message);

        /**
         * @brief Construct a new Network Exception object with error code
         *
         * @param message The error message describing the exception
         * @param errorCode System error code (e.g., errno)
         */
        NetworkException(const std::string &message, int errorCode);

        /**
         * @brief Destroy the Network Exception object
         */
        virtual ~NetworkException() noexcept = default;

        /**
         * @brief Get the exception message
         *
         * @return const char* The error message
         */
        virtual const char *what() const noexcept override;

        /**
         * @brief Get the error code
         *
         * @return int The system error code
         */
        int getErrorCode() const noexcept;

        /**
         * @brief Get the full error message including system error description
         *
         * @return const std::string& Complete error message
         */
        const std::string &getFullMessage() const noexcept;

      protected:
        std::string _message;     ///< The base error message
        std::string _fullMessage; ///< The complete error message with details
        int _errorCode;           ///< System error code
    };

    /**
     * @brief Exception for connection-related errors
     */
    class ConnectionException : public NetworkException {
      public:
        explicit ConnectionException(const std::string &message);
        ConnectionException(const std::string &message, int errorCode);
    };

    /**
     * @brief Exception for socket creation and configuration errors
     */
    class SocketException : public NetworkException {
      public:
        explicit SocketException(const std::string &message);
        SocketException(const std::string &message, int errorCode);
    };

    /**
     * @brief Exception for send operation errors
     */
    class SendException : public NetworkException {
      public:
        explicit SendException(const std::string &message);
        SendException(const std::string &message, int errorCode);
    };

    /**
     * @brief Exception for receive operation errors
     */
    class ReceiveException : public NetworkException {
      public:
        explicit ReceiveException(const std::string &message);
        ReceiveException(const std::string &message, int errorCode);
    };

    /**
     * @brief Exception for timeout errors
     */
    class TimeoutException : public NetworkException {
      public:
        explicit TimeoutException(const std::string &message);
        TimeoutException(const std::string &message, int timeoutMs);
    };

    /**
     * @brief Exception for packet serialization/deserialization errors
     */
    class SerializationException : public NetworkException {
      public:
        explicit SerializationException(const std::string &message);
    };

    /**
     * @brief Exception for invalid packet format or data errors
     */
    class InvalidPacketException : public NetworkException {
      public:
        explicit InvalidPacketException(const std::string &message);
    };

    /**
     * @brief Exception for buffer overflow or underflow errors
     */
    class BufferException : public NetworkException {
      public:
        explicit BufferException(const std::string &message);
    };

} // namespace Client::Exception
