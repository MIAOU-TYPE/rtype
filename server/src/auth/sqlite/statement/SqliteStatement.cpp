/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SqliteStatement
*/

#include "SqliteStatement.hpp"

namespace Db
{
    SqlStatement::SqlStatement(sqlite3 *db, const char *sql)
    {
        if (sqlite3_prepare_v2(db, sql, -1, &_statement, nullptr) != SQLITE_OK) {
            const std::string err = sqlite3_errmsg(db);
            throw SqlStatementError("{SqlStatement::SqlStatement} prepare failed: " + err);
        }
    }

    SqlStatement::~SqlStatement()
    {
        if (_statement)
            sqlite3_finalize(_statement);
    }

    sqlite3_stmt *SqlStatement::raw() const noexcept
    {
        return _statement;
    }
} // namespace Auth
