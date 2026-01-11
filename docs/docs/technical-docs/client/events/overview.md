---
title: Events Overview
---

# Why an Event Bus

An event-driven model allows each subsystem to evolve independently:

- The UI emits **intent** without knowing protocol or networking details
- The networking layer emits **facts** without knowing UI or state logic
- The runtime translates intents into network requests and facts into state transitions

This avoids tight coupling and circular dependencies between systems.

---

## Event Flow Model

1. A system emits an event (`publish`)
2. The event is queued
3. The runtime dispatches events during the update step
4. Subscribed systems react accordingly

Events are disposable and best-effort; no event guarantees a response.

---

## Event Categories

### UI Intent Events
Express user intent only.

Examples:
- `Ui.LoginClicked`
- `Ui.RegisterClicked`
- `Ui.MenuNavigate`

These events do not imply success.

---

### Authentication Events
Reflect authentication outcomes.

Examples:
- `Auth.LoginSucceeded`
- `Auth.LoginFailed`
- `Auth.LogoutSucceeded`

These events always originate from validated network results.

---

### Network Events
Describe network-level state changes.

Examples:
- `Network.Connected(TCP)`
- `Network.Disconnected(UDP)`
- `Network.SnapshotReceived`

They carry facts, not policy.

---

### Gameplay Events
Describe authoritative gameplay transitions.

Examples:
- `Game.RoomJoined`
- `Game.GameStarted`
- `Game.GameEnded`

They may originate from server messages or runtime decisions.

---

## Design Rules

- Events must be explicit and type-safe
- Event payloads must be minimal
- Events must not trigger blocking operations
- Systems must not assume who emitted the event

---

## Invariants

- Events describe state change or intent, never commands
- Events are processed on the main thread
- Event handling must be deterministic per frame
- Dropped events must not break correctness

---
