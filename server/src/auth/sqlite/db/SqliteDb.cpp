/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SqliteDb
*/

#include "SqliteDb.hpp"

namespace Db
{
    SqliteDb::SqliteDb(const std::string &path)
    {
        if (constexpr int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
            sqlite3_open_v2(path.c_str(), &_db, flags, nullptr) != SQLITE_OK) {
            const std::string err = _db ? sqlite3_errmsg(_db) : "unknown";
            if (_db)
                sqlite3_close(_db);
            throw SqliteDbError("SqliteDb open failed: " + err);
        }

        exec("PRAGMA foreign_keys = ON;");
        exec("PRAGMA journal_mode = WAL;");
        exec("PRAGMA synchronous = NORMAL;");
    }

    SqliteDb::~SqliteDb()
    {
        if (_db)
            sqlite3_close(_db);
    }

    void SqliteDb::execUnlocked(const std::string_view sql) const
    {
        const std::string sqlStr(sql);
        char *err = nullptr;

        if (sqlite3_exec(_db, sqlStr.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
            const std::string msg = err ? err : "unknown";
            sqlite3_free(err);
            throw SqliteDbError("SqliteDb exec failed: " + msg);
        }
    }

    void SqliteDb::exec(const std::string_view sql) const
    {
        std::scoped_lock lk(_mtx);
        execUnlocked(sql);
    }

    sqlite3 *SqliteDb::raw() const noexcept
    {
        return _db;
    }

    std::mutex &SqliteDb::mutex() const noexcept
    {
        return _mtx;
    }
} // namespace Auth
