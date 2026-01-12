---
id: auth-tcp-api
title: TCP Auth API
sidebar_label: TCP API
---

# TCP Auth API (server)

Authentication is performed over **TCP**.

Handlers live in:

- `server/src/packet/PacketRouter/TCP/TCPPacketRouter.cpp`

Packet types are defined in the shared protocol:

- `shared/Network/Data/TCP/TCPTypesData.hpp`

## Common TCP framing

All TCP packets share a 5-byte header and a variable-length body:

- `type` (1 byte)
- `requestId` (4 bytes)

The header is built/parsed by:

- `shared/Network/Data/TCP/payload/TCPPayload.*`
- `shared/Network/Data/TCP/payload/writer/TCPWriter.*`
- `shared/Network/Data/TCP/payload/reader/TCPReader.*`

### String encoding (`str16`)

Auth messages use `str16` strings:

- `u16 length` (big-endian)
- `length` bytes of UTF-8 payload

Writers reject strings longer than `65535` bytes.

## Auth-free vs auth-required endpoints

In `TCPPacketRouter::handle` only these packet types are allowed before authentication:

- `HELLO`
- `AUTH_REGISTER`
- `AUTH_LOGIN`

All other TCP calls require an authenticated session; otherwise the server replies:

- `ERROR_MESSAGE` with code `401` and message `AUTH_REQUIRED`

## AUTH_REGISTER

### Request

Type: `AUTH_REGISTER`

Body:

| Field | Type | Notes |
| --- | --- | --- |
| username | `str16` | 3..32 chars; `[A-Za-z0-9_-]` |
| password | `str16` | 6..256 chars |

Trailing bytes are rejected.

### Success response: AUTH_OK

On success the server responds with `AUTH_OK` and also marks the session as authenticated.

## AUTH_LOGIN

### Request

Type: `AUTH_LOGIN`

Body:

| Field | Type | Notes |
| --- | --- | --- |
| username | `str16` | same validation as register |
| password | `str16` | 6..256 chars |

Trailing bytes are rejected.

### Success response: AUTH_OK

On success the server responds with `AUTH_OK` and also marks the session as authenticated.

## AUTH_OK

Type: `AUTH_OK`

Body:

| Field | Type | Notes |
| --- | --- | --- |
| userId | `u32` | SQLite auto-increment ID |
| username | `str16` | canonical username |
| tokenHi | `u32` | upper 32 bits of UDP token |
| tokenLo | `u32` | lower 32 bits of UDP token |
| ttlSec | `u32` | auth TTL in seconds (currently 86400) |

The UDP token is used to bind UDP traffic to the session (see `sessions-and-udp-token.md`).

## ERROR_MESSAGE

Type: `ERROR_MESSAGE`

Body:

| Field | Type |
| --- | --- |
| code | `u16` |
| msg | `str16` |

### Error mapping for auth

The server maps domain errors to “HTTP-like” codes:

| Situation | Code | Message |
| --- | ---:| --- |
| invalid username/password format | 400 | validation message (e.g. `Username must be 3-32...`) |
| username already exists | 409 | `Username already exists` |
| wrong credentials | 401 | `Invalid username or password` |
| internal DB failure | 500 | `Server error (database)` |
| internal crypto failure | 500 | `Server error (crypto)` |
| service unavailable | 500 | `AUTH_LOGIN: service unavailable` / `AUTH_REGISTER: service unavailable` |

### Request correlation

`requestId` is echoed in the response so the client can match it to the request that caused the error.

## Notes on validation (server-side)

Validation is performed in `AuthService`:

- Username: 3..32 characters, limited to `[A-Za-z0-9_-]`.
- Password: 6..256 characters.

