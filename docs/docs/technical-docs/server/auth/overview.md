---
id: auth-overview
title: Authentication Overview
sidebar_label: Overview
---

# Authentication (server)

This document describes the **server-side authentication subsystem**: how user accounts are stored, how passwords are hashed, how TCP login/register works, and how authenticated identity is attached to a network session.

## Where it lives in the codebase

Auth code is grouped under:

- `server/src/auth/`
  - `sqlite/` – minimal SQLite wrappers (`SqliteDb`, `SqliteStatement`)
  - `userStorage/` – database access (`UserStorage`)
  - `password/` – password hashing & verification (`hashPassword`, `verifyPassword`)
  - `service/` – domain logic (`AuthService`)

Integration points:

- `server/src/thread/ServerRuntime.cpp` – creates the DB, initializes schema, builds `AuthService`.
- `server/src/packet/PacketRouter/TCP/TCPPacketRouter.cpp` – handles `AUTH_REGISTER` / `AUTH_LOGIN`, then attaches the identity to the session.
- `server/src/network/SessionManager/Manager/SessionManager.*` – stores the authenticated identity and its TTL.

## High-level responsibilities

### 1) Persistent user storage

The server stores user accounts in a local SQLite database (`data/users.sqlite3`). A small schema migration mechanism uses `PRAGMA user_version`.

### 2) Password hashing

Passwords are never stored in plaintext. The server stores an **encoded scrypt hash** (computed via OpenSSL `EVP_PBE_scrypt`).

### 3) Login and registration API

Auth is exposed through the TCP protocol:

- `AUTH_REGISTER`: create a new account
- `AUTH_LOGIN`: authenticate an existing account
- `AUTH_OK`: server response on success
- `ERROR_MESSAGE`: server response on failure

### 4) Session identity + TTL

Once a user is authenticated, the server binds an `Auth::Identity { userId, username }` to the network session and sets an expiry (TTL). Packet handlers that are not “auth-free” require the session to be authenticated.

## What auth is (and is not) in this project

- This auth system is **account-based** (username/password).
- It is **not** a full authorization model (no roles/permissions).
- It does **not** implement refresh tokens or long-lived sessions; it relies on an in-memory TTL.
- It does **not** encrypt the TCP stream; it assumes a trusted environment or a separate transport layer if needed.

## Typical flow (happy path)

1. Client connects to the TCP server and sends `HELLO`.
2. Server replies with `WELCOME`:
   - server protocol version
   - `sessionId`
   - UDP port to use (`tcpPort + 1`)
   - an initial 64-bit UDP token
3. Client sends either `AUTH_REGISTER` or `AUTH_LOGIN` over TCP.
4. Server validates input, checks/creates the user in SQLite, then replies `AUTH_OK`.
5. Server marks the session as authenticated (identity + TTL) and the client can call the rest of the TCP API.
6. Client sends a UDP `CONNECT` packet containing `sessionId` + token, so the server binds the UDP address to that session.

Related documents:

- **Password hashing**: `password-hashing.md`
- **Database**: `database.md`
- **TCP auth API**: `tcp-api.md`
- **Sessions + UDP token**: `sessions-and-udp-token.md`
