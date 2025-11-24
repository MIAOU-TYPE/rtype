/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkException implementation
*/

#include "NetworkException.hpp"
#include <sstream>

namespace Client
{
    namespace Exception
    {

        NetworkException::NetworkException(const std::string &message)
            : _message(message), _fullMessage(message), _errorCode(0)
        {
        }

        NetworkException::NetworkException(const std::string &message, int errorCode)
            : _message(message), _errorCode(errorCode)
        {
            std::ostringstream oss;
            oss << message;

            if (errorCode != 0) {
                oss << " (Error code: " << errorCode;

                const char *errorStr = std::strerror(errorCode);
                if (errorStr) {
                    oss << " - " << errorStr;
                }
                oss << ")";
            }

            _fullMessage = oss.str();
        }

        const char *NetworkException::what() const noexcept
        {
            return _fullMessage.c_str();
        }

        int NetworkException::getErrorCode() const noexcept
        {
            return _errorCode;
        }

        const std::string &NetworkException::getFullMessage() const noexcept
        {
            return _fullMessage;
        }

        ConnectionException::ConnectionException(const std::string &message)
            : NetworkException("Connection error: " + message)
        {
        }

        ConnectionException::ConnectionException(const std::string &message, int errorCode)
            : NetworkException("Connection error: " + message, errorCode)
        {
        }

        SocketException::SocketException(const std::string &message) : NetworkException("Socket error: " + message)
        {
        }

        SocketException::SocketException(const std::string &message, int errorCode)
            : NetworkException("Socket error: " + message, errorCode)
        {
        }

        SendException::SendException(const std::string &message) : NetworkException("Send error: " + message)
        {
        }

        SendException::SendException(const std::string &message, int errorCode)
            : NetworkException("Send error: " + message, errorCode)
        {
        }

        ReceiveException::ReceiveException(const std::string &message) : NetworkException("Receive error: " + message)
        {
        }

        ReceiveException::ReceiveException(const std::string &message, int errorCode)
            : NetworkException("Receive error: " + message, errorCode)
        {
        }

        TimeoutException::TimeoutException(const std::string &message) : NetworkException("Timeout error: " + message)
        {
        }

        TimeoutException::TimeoutException(const std::string &message, int timeoutMs)
            : NetworkException("Timeout error: " + message + " (timeout: " + std::to_string(timeoutMs) + "ms)", 0)
        {
        }

        SerializationException::SerializationException(const std::string &message)
            : NetworkException("Serialization error: " + message)
        {
        }

        InvalidPacketException::InvalidPacketException(const std::string &message)
            : NetworkException("Invalid packet: " + message)
        {
        }

        BufferException::BufferException(const std::string &message) : NetworkException("Buffer error: " + message)
        {
        }

    } // namespace Exception
} // namespace Client
