---
title: "Overview"
---

# Score & Scoreboard — Overview

## Goal
The client handles two distinct concepts:
- **Live score** (during gameplay): real-time display, transported over UDP.
- **Global scoreboard** (leaderboard): displayed in the menu, transported over TCP.

## 1) Live score (UDP)
### Source
The live score is received through UDP packets (e.g. a `SCORE` message) during the match.

### Parsing
- Validate size: `size == sizeof(ScoreData)`
- Convert endianness:
    - if `ScoreData.score` is `uint32_t` ⇒ **`ntohl()`**
    - if it were `uint16_t` ⇒ `ntohs()`

Correct example (u32):
- `const uint32_t score = ntohl(scoreData.score);`

### Propagation
The UDP router forwards the decoded value to the sink:
- `_sink->onScore(score);`
  Then the game/UI layer updates the on-screen score.

## 2) Global scoreboard (TCP)
### Principle
- The menu triggers a “top N” request.
- The client sends a TCP request.
- The server replies with a structured list (username + score).

### Data
Client-side `ScoreEntry`:
- `username: std::string`
- `score: int`

### Conversion
- The server sends scores as `u32`.
- The client stores them as `int`:
    - clamp to `INT_MAX` if `u32 > INT_MAX`.

## 3) UI → Network → UI flow
### UI trigger
In the menu (scoreboard page or refresh button):
- `_scoreboardRefreshRequested = true`
- `_scoreboardLoading = true` (shows “Loading…”)

### EventBus / Runtime
- `MenuState` detects `wantsScoreboardRefresh()` and emits `ScoreboardGetRequested(limit)`.
- `ClientRuntime` listens to the event and sends `SCOREBOARD_GET` (TCP) with a clamped limit.

### Thread synchronization
The TCP response is handled in the network thread.
The client updates a shared context (`ScoreboardContext`):
- `scores` guarded by a mutex
- `version` (atomic) to signal updates

The UI/render thread reads `version`, copies `scores`, then calls `Menu::setScoreboard(copy)`.

## 4) Limit clamping (safety)
The `limit` is clamped:
- `0` → default value (e.g. 10)
- `> 100` → capped at 100

Reason: avoid oversized payloads, excessive allocations, and abuse/spam.
