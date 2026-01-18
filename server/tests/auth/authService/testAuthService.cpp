/*
** EPITECH PROJECT, 2026
** UDPPacketFactory.cpp
** File description:
** testAuthService
*/

#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>

#include "AuthService.hpp"
#include "Password.hpp"
#include "SqliteDb.hpp"
#include "UserStorage.hpp"

namespace
{
    std::shared_ptr<Auth::UserStorage> makeRepoWithSchema()
    {
        auto db = std::make_shared<Db::SqliteDb>(":memory:");
        auto repo = std::make_shared<Auth::UserStorage>(db);
        repo->initSchema();
        return repo;
    }

    std::shared_ptr<Auth::UserStorage> makeRepoWithoutSchema()
    {
        auto db = std::make_shared<Db::SqliteDb>(":memory:");
        return std::make_shared<Auth::UserStorage>(db);
    }

    TEST(AuthService, CtorThrowsOnNullRepo)
    {
        EXPECT_THROW({ Auth::AuthService svc(nullptr); }, Auth::AuthServiceError);
    }

    TEST(AuthService, RegisterRejectsInvalidUsername)
    {
        Auth::AuthService svc(makeRepoWithSchema());

        try {
            (void) svc.registerUser("ab", "123456");
            FAIL();
        } catch (const Auth::AuthServiceError &e) {
            EXPECT_EQ(e.kind(), "invalid_input");
        }
    }

    TEST(AuthService, RegisterRejectsShortPassword)
    {
        Auth::AuthService svc(makeRepoWithSchema());

        try {
            (void) svc.registerUser("valid", "12345");
            FAIL();
        } catch (const Auth::AuthServiceError &e) {
            EXPECT_EQ(e.kind(), "invalid_input");
        }
    }

    TEST(AuthService, RegisterRejectsTooLongPassword)
    {
        Auth::AuthService svc(makeRepoWithSchema());

        try {
            (void) svc.registerUser("valid", std::string(257, 'x'));
            FAIL();
        } catch (const Auth::AuthServiceError &e) {
            EXPECT_EQ(e.kind(), "invalid_input");
        }
    }

    TEST(AuthService, RegisterThenLoginSuccess)
    {
        Auth::AuthService svc(makeRepoWithSchema());

        const auto okReg = svc.registerUser("player1", "secretpw");
        EXPECT_NE(okReg.userId, 0u);
        EXPECT_EQ(okReg.username, "player1");

        const auto okLogin = svc.login("player1", "secretpw");
        EXPECT_EQ(okLogin.userId, okReg.userId);
        EXPECT_EQ(okLogin.username, "player1");
    }

    TEST(AuthService, RegisterUsernameTaken)
    {
        Auth::AuthService svc(makeRepoWithSchema());

        (void) svc.registerUser("player1", "secretpw");

        try {
            (void) svc.registerUser("player1", "secretpw2");
            FAIL();
        } catch (const Auth::AuthServiceError &e) {
            EXPECT_EQ(e.kind(), "username_taken");
        }
    }

    TEST(AuthService, LoginInvalidCredentialsWhenUserNotFound)
    {
        Auth::AuthService svc(makeRepoWithSchema());

        try {
            (void) svc.login("unknown", "secretpw");
            FAIL();
        } catch (const Auth::AuthServiceError &e) {
            EXPECT_EQ(e.kind(), "invalid_credentials");
        }
    }

    TEST(AuthService, LoginInvalidCredentialsWhenPasswordWrong)
    {
        Auth::AuthService svc(makeRepoWithSchema());

        (void) svc.registerUser("player1", "secretpw");

        try {
            (void) svc.login("player1", "wrongpw");
            FAIL();
        } catch (const Auth::AuthServiceError &e) {
            EXPECT_EQ(e.kind(), "invalid_credentials");
        }
    }

    TEST(AuthService, RegisterMapsSchemaMissingToInternalError)
    {
        Auth::AuthService svc(makeRepoWithoutSchema());

        try {
            (void) svc.registerUser("player1", "secretpw");
            FAIL();
        } catch (const Auth::AuthServiceError &e) {
            EXPECT_EQ(e.kind(), "internal_error");
            EXPECT_NE(std::string(e.what()).find("{AuthService::registerUser}"), std::string::npos);
        }
    }

    TEST(AuthService, LoginMapsSchemaMissingToInternalError)
    {
        Auth::AuthService svc(makeRepoWithoutSchema());

        try {
            (void) svc.login("player1", "secretpw");
            FAIL();
        } catch (const Auth::AuthServiceError &e) {
            EXPECT_EQ(e.kind(), "internal_error");
            EXPECT_NE(std::string(e.what()).find("{AuthService::login}"), std::string::npos);
        }
    }

    TEST(AuthService, LoginRejectsInvalidInput)
    {
        Auth::AuthService svc(makeRepoWithSchema());

        EXPECT_THROW((void) svc.login("ab", "123456"), Auth::AuthServiceError);
        EXPECT_THROW((void) svc.login("valid", "12345"), Auth::AuthServiceError);
        EXPECT_THROW((void) svc.login("valid", std::string(257, 'x')), Auth::AuthServiceError);
    }
} // namespace