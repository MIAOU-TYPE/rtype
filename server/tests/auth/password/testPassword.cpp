/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Password
*/

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "Password.hpp"

namespace {

constexpr const char* kValidPassword = "CorrectHorseBatteryStaple!";
constexpr const char* kWrongPassword = "WrongPassword123";

bool startsWith(const std::string& s, const std::string& prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (c == delim) {
            out.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    out.push_back(cur);
    return out;
}

bool isDecimalU64(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

bool isLowerHex(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        const bool digit = (c >= '0' && c <= '9');
        const bool lower = (c >= 'a' && c <= 'f');
        const bool upper = (c >= 'A' && c <= 'F');
        if (!(digit || lower || upper)) return false;
    }
    return true;
}

}

TEST(AuthPassword, HashPasswordProducesExpectedFormat) {
    const std::string encoded = Auth::hashPassword(kValidPassword);

    ASSERT_TRUE(startsWith(encoded, "scrypt$"));

    const auto parts = split(encoded, '$');
    ASSERT_EQ(parts.size(), 6u) << "Expected format: scrypt$N$r$p$saltHex$dkHex";

    EXPECT_EQ(parts[0], "scrypt");
    EXPECT_TRUE(isDecimalU64(parts[1])) << "N must be decimal";
    EXPECT_TRUE(isDecimalU64(parts[2])) << "r must be decimal";
    EXPECT_TRUE(isDecimalU64(parts[3])) << "p must be decimal";


    EXPECT_EQ(parts[4].size(), 32u);
    EXPECT_TRUE(isLowerHex(parts[4]));


    EXPECT_EQ(parts[5].size(), 64u);
    EXPECT_TRUE(isLowerHex(parts[5]));
}

TEST(AuthPassword, VerifyPasswordAcceptsCorrectPassword) {
    const std::string encoded = Auth::hashPassword(kValidPassword);
    EXPECT_TRUE(Auth::verifyPassword(kValidPassword, encoded));
}

TEST(AuthPassword, VerifyPasswordRejectsWrongPassword) {
    const std::string encoded = Auth::hashPassword(kValidPassword);
    EXPECT_FALSE(Auth::verifyPassword(kWrongPassword, encoded));
}

TEST(AuthPassword, HashPasswordUsesRandomSaltSoHashesDiffer) {
    const std::string a = Auth::hashPassword(kValidPassword);
    const std::string b = Auth::hashPassword(kValidPassword);


    EXPECT_NE(a, b);


    EXPECT_TRUE(Auth::verifyPassword(kValidPassword, a));
    EXPECT_TRUE(Auth::verifyPassword(kValidPassword, b));
}

TEST(AuthPassword, VerifyPasswordRejectsNonScryptTag) {
    const std::string encoded = Auth::hashPassword(kValidPassword);


    std::string tampered = encoded;
    tampered.replace(0, 5, "bcrypt");

    EXPECT_FALSE(Auth::verifyPassword(kValidPassword, tampered));
}

TEST(AuthPassword, VerifyPasswordRejectsMalformedPartCount) {

    EXPECT_FALSE(Auth::verifyPassword(kValidPassword, "scrypt$16384$8$1$deadbeef"));

    EXPECT_FALSE(Auth::verifyPassword(kValidPassword, "scrypt$16384$8$1$00$"));
}

TEST(AuthPassword, VerifyPasswordRejectsNonNumericParams) {

    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$abc$8$1$00$00"));

    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$16384$xx$1$00$00"));

    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$16384$8$yy$00$00"));
}

TEST(AuthPassword, VerifyPasswordRejectsInvalidScryptParamsRangeAndShape) {


    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$20000$8$1$00$00"));

    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$8192$8$1$00$00"));

    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$524288$8$1$00$00"));


    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$16384$0$1$00$00"));
    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$16384$17$1$00$00"));


    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$16384$8$0$00$00"));
    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$16384$8$5$00$00"));
}

TEST(AuthPassword, VerifyPasswordRejectsInvalidHexSaltOrDk) {

    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$16384$8$1$zzzz$00"));
    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$16384$8$1$00$zzzz"));


    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$16384$8$1$00$00"));
    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$16384$8$1$00000000000000000000000000000000$00"));
    EXPECT_FALSE(Auth::verifyPassword("x", "scrypt$16384$8$1$00$0000000000000000000000000000000000000000000000000000000000000000"));
}

TEST(AuthPassword, VerifyPasswordRejectsTamperedHashData) {
    const std::string encoded = Auth::hashPassword(kValidPassword);
    auto parts = split(encoded, '$');
    ASSERT_EQ(parts.size(), 6u);


    std::string dk = parts[5];
    ASSERT_FALSE(dk.empty());
    dk[0] = (dk[0] == '0') ? '1' : '0';
    parts[5] = dk;


    std::string tampered = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        tampered += "$";
        tampered += parts[i];
    }

    EXPECT_FALSE(Auth::verifyPassword(kValidPassword, tampered));
}

TEST(AuthPassword, VerifyPasswordRejectsEmptyEncodedString) {
    EXPECT_FALSE(Auth::verifyPassword(kValidPassword, ""));
}
