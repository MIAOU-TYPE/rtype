# RFC R-Type TCP (RTYP/TCP)

Intended use: Client ↔ Server control-plane over TCP (lobby, auth, rooms, scoreboard)  
Scope: Framed binary messages carrying commands and responses. Not used for real-time game state (UDP does that).

---

## 1. Conventions and terminology

The key words MUST, MUST NOT, REQUIRED, SHALL, SHALL NOT, SHOULD, SHOULD NOT, RECOMMENDED, MAY, and OPTIONAL are to be interpreted as described in RFC 2119.

- Client: Game client.
- Server: Lobby/auth/game coordination server.
- Frame: One complete application message transported over TCP.
- Connection: One TCP stream between a client and the server.

---

## 2. Design goals

1. Reliable control-plane: room/lobby/auth must be reliable and ordered.
2. Simple binary framing: explicit length prefix to delimit messages in a TCP stream.
3. Extensible: versioning and typed messages.
4. Robust: invalid frames MUST be rejected safely.

---

## 3. Transport

### 3.1 TCP only

All control-plane communication defined in this RFC MUST use TCP.

### 3.2 Stream nature

TCP is a byte stream; message boundaries are not preserved. Implementations MUST use framing (§4).

---

## 4. Framing and encoding

### 4.1 Byte order

All multi-byte integers MUST be encoded in network byte order (big-endian).

### 4.2 String encoding

Strings MUST be UTF-8, encoded as:

- `str16` = `uint16 length` followed by `length` bytes (no NUL terminator).

This matches the provided `Reader::str16()` behavior.

### 4.3 Frame format (REQUIRED)

Each message MUST be transmitted as:

| Field | Size | Type | Description |
|---|---:|---|---|
| length | 2 | uint16 | Number of bytes following this field (payload length) |
| payload | length | bytes | Message payload (see §5) |

Constraints:
- `length` MUST be ≥ 2 (at least `type` + `version`) unless otherwise specified.
- Implementations SHOULD cap `length` (RECOMMENDED: 8192 bytes) to avoid memory abuse.
- If the stream ends before `length` bytes are read, the frame is incomplete and MUST be treated as a disconnect.

---

## 5. Common message header (inside payload)

All payloads begin with:

| Field | Size | Type | Description |
|---|---:|---|---|
| type | 1 | uint8 | Message type (see §6) |
| version | 1 | uint8 | Protocol version (this RFC: `0x01`) |

So the minimum `length` is 2.

Receivers MUST drop the connection or reject the frame if:
- `version` unsupported
- `type` unknown (unless §12 extension rules are implemented)
- payload length does not match what the `type` requires

---

## 6. Message type registry

### 6.1 Client → Server

- `0x01` HELLO
- `0x03` LIST_ROOMS
- `0x04` CREATE_ROOM
- `0x05` JOIN_ROOM
- `0x06` LEAVE_ROOM
- `0x07` START_GAME
- `0x08` AUTH_REGISTER
- `0x09` AUTH_LOGIN
- `0x0A` SCOREBOARD_GET
- `0x0B` ROOM_INFO

### 6.2 Server → Client

- `0x10` WELCOME
- `0x11` ERROR_MESSAGE
- `0x12` ROOMS_LIST
- `0x13` ROOM_CREATED
- `0x14` ROOM_JOINED
- `0x15` ROOM_LEFT
- `0x16` ROOM_UPDATE
- `0x17` GAME_START
- `0x18` AUTH_OK
- `0x19` SCOREBOARD_LIST

---

## 7. Connection lifecycle

### 7.1 Recommended flow

1. Client connects via TCP.
2. Client sends `HELLO`.
3. Server replies `WELCOME` or `ERROR_MESSAGE`.
4. Client may then perform auth, room browsing, room operations, and start game.
5. When a game starts, UDP session parameters MAY be provided via `GAME_START` (recommended).

### 7.2 Idempotency

Client MAY resend safe requests after reconnect (LIST_ROOMS, SCOREBOARD_GET). Server MUST handle duplicates safely.

---

## 8. Message formats (v0x01)

Notation:
- All messages include `type` + `version` at the start of payload.
- Fields are big-endian.

### 8.1 HELLO (0x01) — client → server

Purpose: identify client capabilities and optionally a desired username.

Payload:
- type (u8) = 0x01
- version (u8) = 0x01
- clientName (str16) OPTIONAL
- clientBuild (str16) OPTIONAL

If present, fields are read in order; if absent, payload may end after version.

### 8.2 WELCOME (0x10) — server → client

Purpose: acknowledge connection and provide server info.

Payload:
- type = 0x10
- version = 0x01
- serverName (str16) OPTIONAL
- motd (str16) OPTIONAL

### 8.3 ERROR_MESSAGE (0x11) — server → client

Payload:
- type = 0x11
- version = 0x01
- code (u16)
- message (str16)

Recommended error codes:
- 1: BAD_REQUEST
- 2: UNAUTHORIZED
- 3: NOT_FOUND
- 4: CONFLICT
- 5: INTERNAL_ERROR

### 8.4 LIST_ROOMS (0x03) — client → server

Payload:
- type = 0x03
- version = 0x01

### 8.5 ROOMS_LIST (0x12) — server → client

Payload:
- type = 0x12
- version = 0x01
- count (u16)
- repeated `count` times:
  - roomId (u32)
  - roomName (str16)
  - currentPlayers (u16)
  - maxPlayers (u16)

### 8.6 CREATE_ROOM (0x04) — client → server

Payload:
- type = 0x04
- version = 0x01
- roomName (str16)
- maxPlayers (u16)
- gameConfigBlob (bytes) OPTIONAL

`gameConfigBlob` is application-defined. If you need interoperability, define it as explicit fields (recommended).

### 8.7 ROOM_CREATED (0x13) — server → client

Payload:
- type = 0x13
- version = 0x01
- roomId (u32)
- roomName (str16)

### 8.8 JOIN_ROOM (0x05) — client → server

Payload:
- type = 0x05
- version = 0x01
- roomId (u32)

### 8.9 ROOM_JOINED (0x14) — server → client

Payload:
- type = 0x14
- version = 0x01
- roomId (u32)
- roomName (str16)
- currentPlayers (u16)
- maxPlayers (u16)
- playerCount (u16)
- repeated `playerCount` times:
  - playerName (str16)

### 8.10 LEAVE_ROOM (0x06) — client → server

Payload:
- type = 0x06
- version = 0x01

### 8.11 ROOM_LEFT (0x15) — server → client

Payload:
- type = 0x15
- version = 0x01
- roomId (u32)

### 8.12 ROOM_UPDATE (0x16) — server → client

Purpose: push updates when room state changes (players join/leave, settings changed).

Payload:
- type = 0x16
- version = 0x01
- roomId (u32)
- currentPlayers (u16)
- maxPlayers (u16)
- playerCount (u16)
- repeated `playerCount` times:
  - playerName (str16)

### 8.13 ROOM_INFO (0x0B) — client → server

Payload:
- type = 0x0B
- version = 0x01

Server SHOULD respond with `ROOM_UPDATE` (or a dedicated message if you add one later).

### 8.14 START_GAME (0x07) — client → server

Payload:
- type = 0x07
- version = 0x01

Server SHOULD validate permissions (e.g., only host).

### 8.15 GAME_START (0x17) — server → client

Purpose: transition lobby → in-game and deliver UDP session parameters.

Payload:
- type = 0x17
- version = 0x01
- udpHost (str16)  (IP or hostname)
- udpPort (u16)
- roomId (u32)
- matchId (u32) OPTIONAL (recommended for correlating UDP session)

### 8.16 AUTH_REGISTER (0x08) — client → server

Payload:
- type = 0x08
- version = 0x01
- username (str16)
- password (str16)

### 8.17 AUTH_LOGIN (0x09) — client → server

Payload:
- type = 0x09
- version = 0x01
- username (str16)
- password (str16)

### 8.18 AUTH_OK (0x18) — server → client

Payload:
- type = 0x18
- version = 0x01
- userId (u32) OPTIONAL
- username (str16) OPTIONAL
- token (str16) OPTIONAL (session token; if used, define lifetime server-side)

If auth fails, server MUST send `ERROR_MESSAGE` with code UNAUTHORIZED.

### 8.19 SCOREBOARD_GET (0x0A) — client → server

Payload:
- type = 0x0A
- version = 0x01
- limit (u16) OPTIONAL (default 10)

### 8.20 SCOREBOARD_LIST (0x19) — server → client

Payload:
- type = 0x19
- version = 0x01
- count (u16)
- repeated `count` times:
  - username (str16)
  - score (u32)

---

## 9. Validation rules (per-message)

Receivers MUST validate:
- payload has enough bytes for declared fields (use `remaining()` checks)
- counts (`count`, `playerCount`) are capped (RECOMMENDED: ≤ 256)
- strings length is reasonable (RECOMMENDED: ≤ 256 per string)
- any unknown trailing bytes MAY be ignored for forward compatibility, but only if the message is otherwise valid

---

## 10. Security considerations

- Passwords are transported as plaintext in this RFC unless you run TCP over TLS. Production use SHOULD use TLS.
- Servers SHOULD rate-limit AUTH attempts.
- Cap all lengths and counts; reject oversized frames.
- Never trust `roomName`, `username`, `motd`—sanitize for UI.

---

## 11. Backward/forward compatibility

- `version` is per-message payload header.
- Senders MUST set reserved/optional fields consistently.
- Receivers MAY ignore unknown trailing fields to allow additive evolution.

---

## 12. Extension rules

New message types MUST use previously unassigned `type` values.
Existing message types MUST NOT change field order or meaning within the same `version`.

---

## 13. Summary

- TCP stream uses `uint16 length` framing.
- Payload begins with `(type, version)`.
- Strings are `str16` (u16 length + bytes).
- Room and scoreboard lists are counted vectors.
- GAME_START SHOULD include UDP endpoint parameters.
