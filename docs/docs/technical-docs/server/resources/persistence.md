---
title: Persistence
---

#  Persistence

Persistence covers all **durable server-side state** stored outside the process.
It exists to keep critical data across restarts and to support authentication
and long-lived identities.

This document defines what should be persisted, ownership rules, and operational
constraints.

---

## What the Server Persists

### Authentication data (required)
- user accounts (username/email)
- password hashes + per-user salts/parameters
- account status flags (locked, disabled)
- optional metadata (createdAt, lastLoginAt)

Passwords are never stored or logged in plaintext.

### Optional game-related data (project-dependent)
- player stats or progression
- server-side configuration overrides
- audit logs (limited)

If not required, avoid persisting gameplay state to keep the server simpler.

---

## What the Server Should NOT Persist

- plaintext passwords
- raw session tokens (unless explicitly required and securely stored)
- long-lived secrets in logs
- transient gameplay state (entities, positions, snapshots)

Simulation state is ephemeral and should be rebuilt per session.

---

## Ownership and Access Model

Persistence access is owned by the **auth/session layer** (and optionally a
dedicated persistence module). Other subsystems must not talk to the database
directly.

Rules:
- expose narrow interfaces (e.g., `CreateUser`, `VerifyUser`, `GetUserById`)
- do not leak SQL queries into networking or gameplay code
- validate all inputs before calling persistence

---

## Startup Behavior

At startup:
- open database connection (or pool)
- run migrations / schema validation (if applicable)
- fail fast if persistence cannot be initialized

The server must not accept clients if persistence is unavailable.

---

## Runtime Behavior

### Threading rules
- database calls must not happen on network IO threads
- route requests through a worker thread or async task system if needed
- avoid blocking the simulation tick

### Error handling
- transient DB errors should be logged and surfaced as internal errors
- repeated failures may trigger a controlled shutdown (policy dependent)

---

## Schema and Migrations

Recommended rules:
- schema is versioned
- migrations are idempotent
- migrations run before opening network listeners
- incompatible schema versions fail startup with a clear error

---

## Security Requirements

- password hashes use a modern KDF (Argon2id, scrypt, bcrypt)
- store KDF parameters per user (or global, versioned)
- protect against enumeration (avoid overly precise error messages)
- rate-limit login attempts and enforce lockout windows (config-controlled)

Database files must have restricted permissions on disk.

---

## Session Persistence (Optional)

Default recommendation: sessions are **in-memory only**.

If session persistence is implemented:
- store only what is required (session id, user id, expiry)
- use short TTLs
- support invalidation on logout or security events
- rotate tokens when possible

Do not persist UDP bind tokens beyond their short TTL.

---

## Backups and Operational Notes

- backup the auth database regularly (out of scope for runtime code)
- ensure backups are encrypted at rest where applicable
- avoid storing high-volume logs inside the main auth database

---

## Invariants

- Persistence is initialized before accepting clients
- Only minimal necessary data is stored
- Secrets are never logged
- DB access never blocks the simulation tick
- Schema mismatches fail fast at startup
