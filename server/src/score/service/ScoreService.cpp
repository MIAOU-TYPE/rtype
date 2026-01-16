/*
** EPITECH PROJECT, 2025
** ScoreService.cpp
** File description:
** ScoreService
*/

#include "ScoreService.hpp"

namespace Engine
{
    ScoreService::ScoreService(std::shared_ptr<Db::SqliteDb> db) : _db(std::move(db))
    {
        if (!_db)
            throw ScoreServiceError("{ScoreService::ScoreService} db is null");
    }

    void ScoreService::initSchema() const
    {
        std::scoped_lock lk(_db->mutex());

        _db->execUnlocked("CREATE TABLE IF NOT EXISTS scores ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "username TEXT NOT NULL,"
                          "score INTEGER NOT NULL,"
                          "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                          ");");
        _db->execUnlocked("CREATE INDEX IF NOT EXISTS idx_scores_score_desc ON scores(score DESC);");
    }

    void ScoreService::saveScore(const std::string &username, const int score) const
    {
        std::scoped_lock lk(_db->mutex());

        const Db::SqlStatement st(_db->raw(), "INSERT INTO scores(username, score) VALUES(?1, ?2);");
        if (sqlite3_bind_text(st.raw(), 1, username.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
            throw ScoreServiceError("{ScoreService::saveScore} bind username failed");
        if (sqlite3_bind_int(st.raw(), 2, score) != SQLITE_OK)
            throw ScoreServiceError("{ScoreService::saveScore} bind score failed");
        if (sqlite3_step(st.raw()) != SQLITE_DONE)
            throw ScoreServiceError(
                "{ScoreService::saveScore} step failed: " + std::string(sqlite3_errmsg(_db->raw())));
    }

    std::vector<ScoreEntry> ScoreService::getTopScores(const std::size_t limit) const
    {
        std::scoped_lock lk(_db->mutex());

        const Db::SqlStatement st(_db->raw(),
            "SELECT username, score "
            "FROM scores "
            "ORDER BY score DESC "
            "LIMIT ?1;");
        if (sqlite3_bind_int64(st.raw(), 1, static_cast<sqlite3_int64>(limit)) != SQLITE_OK)
            throw ScoreServiceError("{ScoreService::getTopScores} bind limit failed");

        std::vector<ScoreEntry> scores;
        while (true) {
            const int rc = sqlite3_step(st.raw());
            if (rc == SQLITE_DONE)
                break;
            if (rc != SQLITE_ROW)
                throw ScoreServiceError(
                    "{ScoreService::getTopScores} step failed: " + std::string(sqlite3_errmsg(_db->raw())));
            const auto *u = reinterpret_cast<const char *>(sqlite3_column_text(st.raw(), 0));
            const int s = sqlite3_column_int(st.raw(), 1);
            if (!u)
                throw ScoreServiceError("{ScoreService::getTopScores} null username retrieved");
            scores.push_back(ScoreEntry{.username = std::string(u), .score = s});
        }
        return scores;
    }
} // namespace Engine
