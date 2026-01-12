---
id: auth-database
title: Auth Database (SQLite)
sidebar_label: Database
---

# Auth Database (server)

The server persists accounts using **SQLite**.

## Database file location

At startup, `ServerRuntime` ensures a `data/` directory exists, and opens:

- `data/users.sqlite3`

This behavior is implemented in `server/src/thread/ServerRuntime.cpp`.

## SQLite wrapper (`SqliteDb`)

`Auth::SqliteDb` is a thin RAII wrapper around `sqlite3*`:

- Opens the database in `READWRITE | CREATE` mode.
- Applies pragmas:
  - `PRAGMA foreign_keys = ON;`
  - `PRAGMA journal_mode = WAL;`
  - `PRAGMA synchronous = NORMAL;`
- Exposes:
  - `exec(sql)` which locks an internal mutex
  - `execUnlocked(sql)` to run SQL when you already hold the mutex
  - `raw()` to access the underlying `sqlite3*`
  - `mutex()` so higher-level code can implement multi-step operations safely

Thread-safety is achieved with a **single mutex** inside `SqliteDb`.

## Schema initialization and versioning

`UserStorage::initSchema()` is called once at startup.

It uses `PRAGMA user_version` as a very small migration mechanism:

- If `user_version != 0`, initialization returns immediately.
- If `user_version == 0`, it creates the `users` table and sets `user_version = 1`.

### Current schema (v1)

```sql
CREATE TABLE IF NOT EXISTS users (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  username TEXT UNIQUE NOT NULL,
  password_scrypt TEXT NOT NULL,
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
PRAGMA user_version = 1;
```

## Queries

### Find user by username

`UserStorage::findByUsername(username)` executes:

```sql
SELECT id, username, password_scrypt
FROM users
WHERE username = ?1
LIMIT 1;
```

Return type:

- `std::optional<UserRow>` where `UserRow` contains:
  - `Identity { userId, username }`
  - `passwordScrypt`

### Insert user

`UserStorage::insertUser(username, passwordScrypt)` executes:

```sql
INSERT INTO users(username, password_scrypt)
VALUES(?1, ?2);
```

Special case:

- If the username already exists (unique constraint), the function returns `0` (used by `AuthService` to map to `username_taken`).

## Operational notes

- The DB is local to the server instance. If you delete `data/users.sqlite3`, you reset all accounts.
- WAL mode improves concurrency and reduces writer/readers blocking in typical workloads.
- The current design stores only username/password hash. If you add more fields, bump `user_version` and implement a migration path.
