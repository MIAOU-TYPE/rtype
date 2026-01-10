/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AuthService
*/

#include "AuthService.hpp"

#include <utility>

namespace Auth
{
    AuthService::AuthService(std::shared_ptr<UserStorage> repo) : _repo(std::move(repo))
    {
        if (!_repo)
            throw AuthServiceError("internal_error", "null UserStorage");
    }

    bool AuthService::validUsername(const std::string &u)
    {
        if (u.size() < 3 || u.size() > 24)
            return false;
        for (const auto c : u) {
            const bool ok =
                (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-';
            if (!ok)
                return false;
        }
        return true;
    }

    bool AuthService::validPassword(const std::string &p)
    {
        return p.size() >= 6 && p.size() <= 256;
    }

    AuthOk AuthService::registerUser(const std::string &username, const std::string &password) const
    {
        if (!validUsername(username) || !validPassword(password))
            throw AuthServiceError("invalid_input", "{AuthService::registerUser} validation failed");

        try {
            const std::uint32_t id = _repo->insertUser(username, hashPassword(password));
            if (id == 0)
                throw AuthServiceError("username_taken", "{AuthService::registerUser} username already exists");
            return AuthOk{id, username};
        } catch (const AuthServiceError &) {
            throw;
        } catch (const UserStorageError &e) {
            throw AuthServiceError("db_error", std::string("{AuthService::registerUser} ") + e.what());
        } catch (const PasswordError &e) {
            throw AuthServiceError("crypto_error", std::string("{AuthService::registerUser} ") + e.what());
        } catch (const std::exception &e) {
            throw AuthServiceError("internal_error", std::string("{AuthService::registerUser} ") + e.what());
        }
    }

    AuthOk AuthService::login(const std::string &username, const std::string &password) const
    {
        if (!validUsername(username) || !validPassword(password))
            throw AuthServiceError("invalid_input", "{AuthService::login} validation failed");

        try {
            const auto row = _repo->findByUsername(username);
            if (!row || !verifyPassword(password, row->passwordScrypt))
                throw AuthServiceError("invalid_credentials", "{AuthService::login} invalid username or password");
            return AuthOk{row->identity.userId, row->identity.username};
        } catch (const AuthServiceError &) {
            throw;
        } catch (const UserStorageError &e) {
            throw AuthServiceError("db_error", std::string("{AuthService::login} ") + e.what());
        } catch (const PasswordError &e) {
            throw AuthServiceError("crypto_error", std::string("{AuthService::login} ") + e.what());
        } catch (const std::exception &e) {
            throw AuthServiceError("internal_error", std::string("{AuthService::login} ") + e.what());
        }
    }
} // namespace Auth
