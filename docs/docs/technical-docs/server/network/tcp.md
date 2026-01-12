---
title: TCP Channel
sidebar_label: TCP
---

# TCP Channel

TCP is the server **control plane**. It is used for low-frequency, high-importance
operations that require reliability, ordering, and explicit confirmation.

This document describes what belongs to TCP on the server, how messages are
handled, and what failure semantics must be enforced.

---

## Role of TCP on the Server

TCP is used when the server must guarantee:
- delivery (no silent loss)
- ordering
- clear request/response semantics
- explicit error reporting

TCP is not used for real-time replication; snapshots remain on UDP.

---

## Typical Use Cases

TCP handles:
- authentication (`REGISTER`, `LOGIN`, `LOGOUT`, session resume if supported)
- session establishment and metadata exchange
- room listing / room creation
- join / leave room confirmation
- game phase signals (start/end) when they must be delivered reliably
- error messages tied to a specific request

TCP messages define authoritative control state.

---

## Framing and Parsing

### Framing
TCP is stream-based, so the server must implement framing.

Recommended framing:
- `[u32 size (network endian)][payload bytes...]`

Rules:
- reject size `0`
- enforce a hard maximum frame size
- handle partial frames (accumulate until complete)
- handle multiple frames in one read

### Parsing
- parse header first (type, requestId, etc.)
- parse body using a bounded reader
- reject truncated or malformed payloads
- never allocate unbounded memory based on client input

Malformed frames must result in a disconnect.

---

## Request / Response Semantics

### Request IDs
Every client request must include a `requestId`.

Server rules:
- responses echo the same `requestId`
- errors are returned as `ERROR_MESSAGE(requestId, code, message)`
- unknown request types receive a protocol error (or disconnect)

Client rules (assumed):
- client matches responses using requestId
- UI remains non-blocking

### Idempotency
- Some requests can be treated as idempotent (e.g., list rooms)
- Others are not (e.g., register)

If retry semantics exist, they must be explicit and documented.

---

## Message Handling Model

A typical server flow:

1. TCP accept creates a per-client connection context
2. Reads are non-blocking (or event-loop driven)
3. Incoming frames are parsed and routed by message type
4. Handlers validate:
    - authentication state
    - permissions
    - room membership
    - request payload integrity
5. Handlers emit internal events or call owning services
6. Responses are queued to the connection TX buffer

No handler may block on disk or long operations in the IO thread.

---

## Authentication Gate

Most TCP operations require an authenticated session.

Rules:
- `LOGIN` / `REGISTER` allowed when unauthenticated
- `LIST_ROOMS`, `CREATE_ROOM`, `JOIN_ROOM`, etc. require authenticated session
- on session expiration: reject requests and force re-authentication

If the session is invalid:
- respond with an auth error
- optionally disconnect (policy dependent)

---

## Keep-Alive and Timeouts

If keep-alive is required:

- server expects periodic keep-alive or traffic
- server tracks last-activity time per connection
- if idle beyond timeout window:
    - close connection
    - cleanup session state as needed

The server must also protect itself:
- cap handshake retries
- cap unauthenticated connection lifetime (anti-abuse)

---

## Disconnect Semantics

On TCP disconnect:
- the server considers the client control plane dead
- room membership may be revoked immediately or after a grace period
- session state may be invalidated (policy dependent)
- UDP packets from that endpoint/session must be rejected (no control plane)

Disconnect handling must be idempotent.

---

## Backpressure and TX Queueing

TCP is reliable, so the server must implement backpressure:

- maintain a bounded TX buffer per connection
- if TX buffer grows too large:
    - drop the connection (preferred)
    - or shed low-priority messages (only if safe)

Never allow unbounded memory growth due to a slow client.

---

## Security Rules

- validate all lengths and types
- enforce maximum sizes on strings and arrays
- rate-limit authentication attempts
- avoid revealing sensitive details in error messages
- never log passwords or raw secrets

If transport security exists in the stack, TCP should use it.

---

## Invariants

- TCP handles authoritative control state only
- Every request has an explicit response or error
- Malformed input results in safe rejection and/or disconnect
- IO threads never block on long operations
- Backpressure is enforced to protect server stability
