/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPWriter
*/

#pragma once
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "Endian.hpp"
#include <string_view>

namespace Net::TCP
{
    /**
     * @class WriterError
     * @brief Exception class for Writer errors
     */
    class WriterError : public std::exception {
      public:
        /**
         * @brief Construct a new WriterError object
         * @param message The error message
         */
        explicit WriterError(std::string message) : _message(std::move(message))
        {
        }

        /**
         * @brief Get the error message
         * @return The error message
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    /**
     * @brief Writer class for serializing data into a byte buffer
     * Provides methods to write various data types into an internal byte vector.
     * Supports writing uint8_t, uint16_t, uint32_t, and length-prefixed strings.
     * Handles endianness for multibyte types.
     */
    class Writer {
      public:
        /**
         * @brief Write an 8-bit unsigned integer to the buffer.
         * @param v The value to write.
         */
        void u8(uint8_t v);

        /**
         * @brief Write a 16-bit unsigned integer to the buffer in network byte order.
         * @param v The value to write.
         */
        void u16(uint16_t v);

        /**
         * @brief Write a 32-bit unsigned integer to the buffer in network byte order.
         * @param v The value to write.
         */
        void u32(uint32_t v);

        /**
         * @brief Write a length-prefixed string to the buffer.
         * @param s The string to write.
         */
        void str16(std::string_view s);

        /**
         * @brief Get the internal byte buffer.
         * @return Reference to the byte vector.
         */
        std::vector<uint8_t> &bytes() noexcept;

        /**
         * @brief Get the internal byte buffer (const version).
         * @return Const reference to the byte vector.
         */
        const std::vector<uint8_t> &bytes() const noexcept;

      private:
        /**
         * @brief Push raw bytes into the buffer.
         * @param p Pointer to the data to push.
         * @param n Number of bytes to push.
         */
        void push(const void *p, size_t n);

        std::vector<uint8_t> _buf; ///> Internal byte buffer
    };
} // namespace Net::TCP
