/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPReader
*/

#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>

namespace Net::TCP
{
    /**
     * @brief Reader class for deserializing data from a byte buffer
     * Provides methods to read various data types from a byte array.
     * Supports reading uint8_t, uint16_t, uint32_t, and length-prefixed strings.
     * Handles endianness for multibyte types.
     */
    class Reader {
      public:
        /**
         * @brief Construct a new Reader object
         * @param p Pointer to the byte array
         * @param n Size of the byte array
         */
        Reader(const uint8_t *p, size_t n);

        /**
         * @brief Read an 8-bit unsigned integer from the buffer
         * @return The read value
         */
        [[nodiscard]] uint8_t u8() noexcept;

        /**
         * @brief Read a 16-bit unsigned integer from the buffer
         * @return The read value
         */
        [[nodiscard]] uint16_t u16() noexcept;

        /**
         * @brief Read a 32-bit unsigned integer from the buffer
         * @return The read value
         */
        [[nodiscard]] uint32_t u32() noexcept;

        /**
         * @brief Read a length-prefixed string from the buffer
         * @return The read string
         */
        [[nodiscard]] std::string str16() noexcept;

        /**
         * @brief Get the number of remaining bytes in the buffer
         * @return The number of remaining bytes
         */
        [[nodiscard]] size_t remaining() const noexcept;

      private:
        /**
         * @brief Ensure that there are at least n bytes remaining in the buffer
         * @param n Number of bytes needed
         */
        [[nodiscard]] bool need(size_t n) const noexcept;

        const uint8_t *_cursor; ///> Current read position
        const uint8_t *_end;    ///> End of the buffer
    };
} // namespace Net::TCP