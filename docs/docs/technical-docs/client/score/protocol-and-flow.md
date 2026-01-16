---
title: "Protocol & Flow"
---

# Scoreboard  — TCP Protocol & Flow

## TCP messages

### Client → Server: SCOREBOARD_GET (type = 0x0A)
**Purpose**: request the global top N scores.

Typical payload:
- `limit: u16`

Client rules:
- clamp `limit` (e.g. 1..100)
- generate a unique `requestId`

### Server → Client: SCOREBOARD_LIST (type = 0x19)
**Purpose**: return `count` scoreboard entries.

Payload:
- `count: u16`
- repeated `count` times:
    - `username: str16`
    - `score: u32`

Client parsing steps:
1. Read `count`
2. `scores.reserve(count)` **once before the loop**
3. For each entry:
    - `username = r.str16()`
    - `s = r.u32()`
    - convert `u32 -> int` (clamp if needed)
4. Verify `r.remaining() == 0` (no trailing bytes)
5. `_sink->onScoreboardList(req, scores)`

## Best practice: reserve() outside the loop
Incorrect:
- calling `scores.reserve(count)` inside the loop

Correct:
- call `scores.reserve(count)` immediately after creating the vector

## End-to-end flow (menu)

### 1) UI
- Entering the scoreboard page or pressing refresh:
    - the menu switches to “Loading…”

### 2) EventBus
- `MenuState`:
    - if `wantsScoreboardRefresh()` ⇒ emit `ScoreboardGetRequested(limit)`
    - then `consumeScoreboardRefresh()`

### 3) TCP request
- `ClientRuntime`:
    - clamp `limit`
    - `makeScoreboardGet(reqId, limit)`
    - `sendPacket(pkt)`

### 4) TCP response
- TCP thread:
    - receives `SCOREBOARD_LIST`
    - parses in `TCPPacketRouter::onScoreboardList`
    - triggers callback `onScoreboardList(reqId, scores)`

### 5) Synchronization to UI
- Write into `ScoreboardContext`:
    - lock mutex, set `scores`
    - `version++`
- `MenuState`:
    - if `version` changed ⇒ copy `scores` (lock) ⇒ `Menu::setScoreboard(copy)`
    - clear the “Loading…” state

## Note on score persistence
The displayed scoreboard depends on the server persistence policy.
The client does not require a dedicated ACK if the server persists scores:
- either via an explicit submit message,
- or on `LEAVE_ROOM` / disconnect server-side.

The client can simply refresh the scoreboard after relevant actions.
