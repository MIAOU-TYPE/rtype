/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkException implementation
*/

#include "NetworkException.hpp"
#include <sstream>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <cstring>
#endif

namespace Client::Exception
{

    /**
     * @brief Get error message from system error code in a cross-platform way
     *
     * @param errorCode System error code
     * @return std::string Error message
     */
    static std::string getSystemErrorMessage(int errorCode)
    {
#ifdef _WIN32
        if (errorCode == 0) {
            return "";
        }

        char *messageBuffer = nullptr;
        size_t size =
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &messageBuffer, 0, NULL);

        std::string message;
        if (size > 0 && messageBuffer != nullptr) {
            message = std::string(messageBuffer, size);
            // Remove trailing newlines
            while (!message.empty() && (message.back() == '\n' || message.back() == '\r')) {
                message.pop_back();
            }
        }

        if (messageBuffer) {
            LocalFree(messageBuffer);
        }

        return message.empty() ? "Unknown error" : message;
#else
        if (errorCode == 0) {
            return "";
        }

        const char *errorStr = std::strerror(errorCode);
        return errorStr ? std::string(errorStr) : "Unknown error";
#endif
    }

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

            std::string errorStr = getSystemErrorMessage(errorCode);
            if (!errorStr.empty()) {
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

} // namespace Client::Exception
