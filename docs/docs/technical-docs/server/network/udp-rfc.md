# RFC R-Type UDP (RTYP/UDP)

Status: Draft
Intended use: Client ↔ Authoritative Server real-time game networking over UDP
Scope: Binary UDP protocol for connection, inputs, snapshots, and game events. 

---

## 1. Conventions and terminology

The key words MUST, MUST NOT, REQUIRED, SHALL, SHALL NOT, SHOULD, SHOULD NOT, RECOMMENDED, MAY, and OPTIONAL are to be interpreted as described in RFC 2119.

* Client: Game client instance controlled by a player.
* Server: Authoritative game server.
* Datagram: A single UDP packet payload.
* Tick: Server simulation step (monotonic increasing integer).
* Sequence: Monotonic increasing per-sender packet counter (uint32).

---

## 2. Design goals

1. Real-time: Prioritize freshness over reliability for high-rate state updates (snapshots).
2. Authoritative server: Server is source of truth for game state and gameplay events.
3. Binary & compact: Fixed headers, packed structs, optional compression.
4. Robustness: Malformed packets MUST NOT crash either side.
5. Extensible: Versioning and reserved fields allow future evolution.

---

## 3. Transport

### 3.1 UDP only

All in-game communication MUST use UDP.

### 3.2 MTU considerations

To avoid IP fragmentation, implementations SHOULD keep UDP payload size ≤ 1200 bytes (safe across typical Internet paths). Larger logical messages MUST be chunked at the application layer (see §8).

---

## 4. Byte order, alignment, and encoding

### 4.1 Byte order

All multi-byte integer fields MUST be encoded in network byte order (big-endian):

* uint16: big-endian
* uint32: big-endian

Single-byte fields are unchanged.

### 4.2 Struct packing

Wire layout is byte-packed (no padding). Implementations MUST NOT serialize by raw `memcpy` of in-memory structs unless they also guarantee:

* pack(1) layout
* big-endian conversion for multi-byte fields
* identical type widths

### 4.3 Floating point

`float` MUST NOT be sent on the wire in this protocol version. Positions are quantized (see §8.4).

---

## 5. Common packet header

All packets begin with HeaderData:

| Field    | Size | Type    | Description                                    |
| -------- | ---: | ------- | ---------------------------------------------- |
| magic    |    4 | byte[4] | MUST be ASCII `"RTYP"`                         |
| type     |    1 | uint8   | Packet type (see §6)                           |
| version  |    1 | uint8   | Protocol version (this RFC: `0x01`)            |
| size     |    2 | uint16  | Total datagram size in bytes, including header |
| sequence |    4 | uint32  | Per-sender monotonically increasing sequence   |

Total: 12 bytes

### 5.1 Validation rules

A receiver MUST drop the datagram if:

* `magic != "RTYP"`
* `version` unsupported
* `size < 12` or `size > received_payload_length`
* `type` unknown (unless §13 extension rules are implemented)

### 5.2 Sequence usage

* Each sender maintains its own `sequence` counter starting at 1.
* Receivers MAY use `sequence` to discard duplicates and detect reordering.
* `sequence` is not an ACK by itself (see §10 for reliability policy).

---

## 6. Packet type registry

### 6.1 Client → Server

* `0x01` CONNECT
* `0x02` DISCONNECT
* `0x03` INPUT
* `0x04` PING

### 6.2 Server → Client

* `0x10` ACCEPT
* `0x11` REJECT
* `0x12` SNAPSHOT_RAW
* `0x13` SNAPSHOT_COMPRESSED
* `0x14` PONG
* `0x15` DAMAGE_EVENT
* `0x16` GAME_END
* `0x17` SCORE
* `0x18` DESTROY_ENTITY
* `0x19` HEALTH

---

## 7. Connection model (UDP session)

UDP is connectionless; this protocol defines an application-layer session.

### 7.1 Endpoint identity

A client session is identified by `(client_ip, client_port)` tuple as seen by the server.

### 7.2 State machine (server-side)

1. NEW: unknown endpoint
2. PENDING: CONNECT received, awaiting acceptance
3. ACTIVE: ACCEPT sent; process INPUT; send snapshots/events
4. CLOSING: DISCONNECT received or timeout; stop sending; clean up

### 7.3 Timeouts

* Server SHOULD consider a client timed out if no valid packet is received for T_timeout = 5s (configurable).
* Client SHOULD consider server unreachable if no valid packet is received for T_timeout = 5s.

---

## 8. World state snapshots

Snapshots are server → client messages containing entity state.

### 8.1 Snapshot chunking

Snapshots MAY be split across multiple datagrams using `chunkIndex` / `chunkCount`. A client reconstructs a snapshot for a given `serverTick` once all chunks `[0..chunkCount-1]` are received.

If some chunks are missing after a short window, the client SHOULD discard the partial snapshot and continue with newer ticks.

### 8.2 SnapshotBatchHeader (RAW)

Wire layout:

| Field      | Size | Type       |
| ---------- | ---: | ---------- |
| header     |   12 | HeaderData |
| count      |    2 | uint16     |
| serverTick |    4 | uint32     |
| chunkIndex |    2 | uint16     |
| chunkCount |    2 | uint16     |

Total header: 22 bytes

Payload: `count` repetitions of `SnapshotEntityData` (10 bytes each).
Total size MUST satisfy: `size == 22 + count * 10`.

### 8.3 SnapshotCompressedHeader (COMPRESSED)

Wire layout:

| Field      | Size | Type       |
| ---------- | ---: | ---------- |
| header     |   12 | HeaderData |
| count      |    2 | uint16     |
| serverTick |    4 | uint32     |
| chunkIndex |    2 | uint16     |
| chunkCount |    2 | uint16     |
| rawSize    |    2 | uint16     |
| compSize   |    2 | uint16     |

Total header: 26 bytes

Payload: `compSize` bytes of compressed data.

### 8.4 SnapshotEntityData

Wire layout (10 bytes):

| Field    | Size | Type   | Meaning      |
| -------- | ---: | ------ | ------------ |
| id       |    4 | uint32 | Entity ID    |
| x        |    2 | int16  | Quantized X  |
| y        |    2 | int16  | Quantized Y  |
| z        |    1 | uint8  | Layer/depth  |
| spriteId |    1 | uint8  | Sprite index |

#### 8.4.1 Position quantization

`x` and `y` represent fixed-point positions:

* world_units = int16_value / 100.0
* Example: `x = 1234` → `12.34` world units

This scale factor is part of protocol version `0x01`. Changing it requires bumping `version`.

### 8.5 Compression algorithm

For `SNAPSHOT_COMPRESSED`, the compressed payload MUST be LZ4 block format (raw block, not frame), compressing the RAW snapshot payload bytes that would follow `SnapshotBatchHeader`:

* Uncompressed source = `count * sizeof(SnapshotEntityData)` bytes
* `rawSize` MUST equal that source length
* `compSize` MUST equal compressed payload length

If compression fails or expands, server SHOULD send RAW instead.

---

## 9. Player inputs

### 9.1 PlayerInputData (Client → Server, type 0x03)

Wire layout:

| Field  | Size | Type       |
| ------ | ---: | ---------- |
| header |   12 | HeaderData |
| flags  |    1 | uint8      |

Flags bits:

* bit0: Up
* bit1: Down
* bit2: Left
* bit3: Right
* bit4: Shoot
* bit5: PowerShoot
* bit6..7: reserved (MUST be 0)

### 9.2 Input rate and semantics

* Client SHOULD send INPUT at a fixed rate (e.g., 30–60 Hz) or when state changes.
* Server processes INPUTs in order of `sequence` when possible; out-of-order packets MAY be dropped if too old.

---

## 10. Ping / Pong (latency)

### 10.1 PING (Client → Server, type 0x04)

Format: `DefaultData` (header only).
Purpose: solicit a PONG response.

### 10.2 PONG (Server → Client, type 0x14)

Wire layout:

| Field         | Size | Type       |
| ------------- | ---: | ---------- |
| header        |   12 | HeaderData |
| pongTimestamp |    4 | uint32     |

`pongTimestamp` is an echo token set by the server. Recommended behavior:

* Client sends PING with `sequence = S`
* Server replies PONG with `pongTimestamp = S`
* Client measures RTT using local send/receive times and matches by `pongTimestamp`

(If you prefer “client timestamp echo”, add a new packet version/type; do not overload fields silently.)

---

## 11. Score and health

### 11.1 SCORE (Server → Client, type 0x17)

Wire layout:

| Field    | Size | Type       |
| -------- | ---: | ---------- |
| header   |   12 | HeaderData |
| playerId |    4 | uint32     |
| score    |    4 | uint32     |

### 11.2 HEALTH (Server → Client, type 0x19)

Wire layout:

| Field       | Size | Type       |
| ----------- | ---: | ---------- |
| header      |   12 | HeaderData |
| currentLife |    2 | uint16     |
| maxLife     |    2 | uint16     |

---

## 12. Entity lifecycle and game events

The following packet types are defined by this RFC but require payload definitions to be interoperable. If you already have internal structs, align them to these rules.

### 12.1 DESTROY_ENTITY (0x18)

Purpose: remove an entity from the client scene.

Wire layout (REQUIRED for v0x01):

* header (12)
* entityId (uint32)

`size` MUST be 16.

### 12.2 DAMAGE_EVENT (0x15)

Purpose: notify damage application.

Wire layout (REQUIRED for v0x01):

* header (12)
* targetEntityId (uint32)
* amount (uint16)
* sourceEntityId (uint32) (0 if unknown)
* flags (uint8) (bit0: critical, bit1: lethal, others 0)

`size` MUST be 12+4+2+4+1 = 23.

### 12.3 GAME_END (0x16)

Purpose: end of round/game.

Wire layout (REQUIRED for v0x01):

* header (12)
* reason (uint8) (0: server_shutdown, 1: victory, 2: defeat, 3: timeout, 4: aborted)
* winnerPlayerId (uint32) (0 if none)

`size` MUST be 17.

(If you need alignment, do not add padding; change field order or bump version.)

---

## 13. Versioning and forward compatibility

* `version` in HeaderData is the protocol version.
* A receiver that does not support a version MUST drop the packet.
* Reserved bits/fields MUST be set to 0 by senders and MUST be ignored by receivers.

---

## 14. Reliability policy (what must be reliable)

UDP is unreliable; this RFC defines what MUST be “effectively reliable” and how to achieve it.

### 14.1 Reliability classes

* Unreliable / latest-wins: SNAPSHOT_* (0x12/0x13), INPUT (0x03)
* Should be reliable: CONNECT/ACCEPT/REJECT, DISCONNECT, GAME_END, DESTROY_ENTITY
* Optional reliability: SCORE, HEALTH, DAMAGE_EVENT (depends on your gameplay)

### 14.2 Recommended strategy (minimal)

For “should be reliable” messages:

* Send the message N times (e.g., N=3) over a short interval (e.g., 100ms), each with increasing `sequence`.
* Receiver deduplicates using `(type, semantic key)`:

  * DESTROY_ENTITY: key = entityId
  * GAME_END: key = serverTick or “game instance id” if you add one
  * ACCEPT: key = client endpoint

This avoids building a full ACK system while achieving practical reliability.

(If you implement ACKs, define new packet types; do not retrofit into existing payloads.)

---

## 15. Security and robustness requirements

1. Size checks: never allocate based solely on packet fields; cap `count`, `chunkCount`, `rawSize`, `compSize`.

   * RECOMMENDED caps:

     * `count ≤ 1024`
     * `chunkCount ≤ 64`
     * `rawSize ≤ 65535` and also ≤ application limit (e.g., 8192)
2. Decompression limits: refuse decompression if `rawSize` exceeds cap or if decompressor reports overflow.
3. Rate limiting: server SHOULD rate-limit NEW endpoints (CONNECT floods) by IP.
4. State isolation: malformed packets MUST NOT mutate authoritative game state.
5. Replay/duplication: accept that UDP can duplicate; always dedupe by `sequence` where applicable.

---

## 16. Summary of wire formats (v0x01)

* HeaderData: 12 bytes, magic `"RTYP"`, big-endian integers
* CONNECT/DISCONNECT/PING: header only (12)
* PONG: 16 bytes
* INPUT: 13 bytes
* SNAPSHOT_RAW: 22 + 10*count
* SNAPSHOT_COMPRESSED: 26 + compSize
* SCORE: 20 bytes
* HEALTH: 16 bytes
* DESTROY_ENTITY: 16 bytes
* DAMAGE_EVENT: 23 bytes
* GAME_END: 17 bytes

---
