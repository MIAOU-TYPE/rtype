/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testEndian
*/

#include <cmath>
#include <gtest/gtest.h>
#include <limits>
#include "Endian.hpp"

TEST(EndianTests, BigEndian64RoundTrip)
{
    uint64_t values[] = {0ULL, 1ULL, 0x1122334455667788ULL, 0xFFEEDDCCBBAA9988ULL, UINT64_MAX};

    for (uint64_t v : values) {
        uint64_t be = htobe64(v);
        uint64_t ret = be64toh(be);
        EXPECT_EQ(ret, v) << "Failed round-trip for value: " << v;
    }
}

TEST(EndianTests, FloatRoundTrip)
{
    float values[] = {
        0.0f,
        1.0f,
        -1.0f,
        3.1415926f,
        -12345.678f,
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
    };

    for (float f : values) {
        uint32_t encoded = htonf(f);
        float decoded = ntohf(encoded);

        if (std::isinf(f)) {
            EXPECT_TRUE(std::isinf(decoded));
            EXPECT_EQ(std::signbit(f), std::signbit(decoded));
        } else {
            EXPECT_FLOAT_EQ(decoded, f) << "Failed float round-trip for value: " << f;
        }
    }
}
