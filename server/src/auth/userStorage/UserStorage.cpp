/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UserStorage
*/

#include "UserStorage.hpp"

namespace Auth
{
    UserStorage::UserStorage(std::shared_ptr<SqliteDb> db) : _db(std::move(db))
    {
        if (!_db)
            throw UserStorageError("{UserStorage::UserStorage} null SqliteDb");
    }

    void UserStorage::initSchema() const
    {
        std::scoped_lock lk(_db->mutex());

        const SqlStatement st(_db->raw(), "PRAGMA user_version;");
        if (sqlite3_step(st.raw()) != SQLITE_ROW)
            throw UserStorageError("{UserStorage::initSchema} step failed: " + std::string(sqlite3_errmsg(_db->raw())));
        if (const int version = sqlite3_column_int(st.raw(), 0); version != 0)
            return;

        _db->execUnlocked("CREATE TABLE IF NOT EXISTS users ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "username TEXT UNIQUE NOT NULL,"
                          "password_scrypt TEXT NOT NULL,"
                          "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                          ");");
        _db->execUnlocked("PRAGMA user_version = 1;");
    }

    std::optional<UserRow> UserStorage::findByUsername(const std::string &username) const
    {
        std::scoped_lock lk(_db->mutex());

        const SqlStatement st(_db->raw(),
            "SELECT id, username, password_scrypt "
            "FROM users WHERE username = ?1 LIMIT 1;");
        if (sqlite3_bind_text(st.raw(), 1, username.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
            throw UserStorageError("{UserStorage::findByUsername} bind failed");
        const int rc = sqlite3_step(st.raw());
        if (rc == SQLITE_DONE)
            return std::nullopt;
        if (rc != SQLITE_ROW)
            throw UserStorageError("{UserStorage::findByUsername} step failed");
        const auto *dbUsername = reinterpret_cast<const char *>(sqlite3_column_text(st.raw(), 1));
        const auto *dbPassword = reinterpret_cast<const char *>(sqlite3_column_text(st.raw(), 2));
        if (!dbUsername || !dbPassword)
            throw UserStorageError("{UserStorage::findByUsername} NULL column");

        UserRow user{
            .identity =
                Identity{
                    .userId = static_cast<std::uint32_t>(sqlite3_column_int(st.raw(), 0)),
                    .username = dbUsername,
                },
            .passwordScrypt = dbPassword,
        };
        return user;
    }

    std::uint32_t UserStorage::insertUser(const std::string &username, const std::string &passwordScrypt) const
    {
        std::scoped_lock lk(_db->mutex());

        const SqlStatement st(_db->raw(), "INSERT INTO users(username, password_scrypt) VALUES(?1, ?2);");
        if (sqlite3_bind_text(st.raw(), 1, username.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK
            || sqlite3_bind_text(st.raw(), 2, passwordScrypt.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
            throw UserStorageError("insertUser: bind failed");
        if (sqlite3_step(st.raw()) != SQLITE_DONE) {
            if (sqlite3_extended_errcode(_db->raw()) == SQLITE_CONSTRAINT_UNIQUE)
                return 0;
            throw UserStorageError("insertUser: step failed");
        }
        return static_cast<std::uint32_t>(sqlite3_last_insert_rowid(_db->raw()));
    }

} // namespace Auth
