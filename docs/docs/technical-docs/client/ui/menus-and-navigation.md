---
title: Menus and Navigation
---

# Menus and Navigation

This document defines the **navigation model** of the client and the rules that
govern transitions between screens. Navigation is driven by **events and
runtime state**, not by direct UI decisions.

---

## Navigation Graph

The client follows a simple, explicit screen graph:

- `Boot` → `MainMenu`
- `MainMenu` → `Login` / `Register` / `Settings`
- `Login` → `Lobby` (on successful authentication)
- `Lobby` → `Room` / `MainMenu`
- `Room` → `InGame` (when the server starts the game)
- `InGame` → `Lobby` (on normal game end)
- `InGame` → `MainMenu` (on disconnect or fatal error)

Transitions not listed here are forbidden.

---

## Transition Ownership

### UI responsibility
- Render the current screen
- Emit intent events:
    - `LoginRequested`
    - `JoinRoomRequested`
    - `LeaveRoomRequested`
- Display feedback and errors

The UI never performs transitions directly.

---

### Runtime responsibility
- Interpret intent events
- Communicate with the server
- Perform state transitions only after authoritative confirmation

This guarantees consistency between UI and server state.

---

## Authoritative Transitions

Some transitions are **server-driven**:

- `Lobby` → `Room`: join accepted
- `Room` → `InGame`: game start signal
- `InGame` → `Lobby`: game end signal

The client must wait for these signals before transitioning.

---

## Error and Edge Transitions

### Network failures
- Any TCP disconnect → `MainMenu`
- Display a global error banner
- Clear session and room state

### Authentication errors
- Authentication failure → `Login`
- Token invalidation → `Login`

### Room-related errors
- Room full or join rejected → remain in `Lobby`
- Kick or forced leave → return to `Lobby` with message

---

## Transition Rules

- All transitions occur on the main thread
- Transitions are atomic
- Partial transitions are not allowed
- UI always reflects runtime state

---

## Invariants

- Navigation is deterministic
- UI never bypasses server authority
- Errors always result in a valid screen
- No transition depends on rendering or frame timing
