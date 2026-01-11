---
title: Game Overview
---

# Server Game Model Overview

This document provides a **high-level view of the server-side game model**.
It explains what the server considers a “game”, how it fits into the overall
architecture, and how gameplay state is owned, advanced, and exposed to clients.

This is a conceptual overview, not a low-level implementation description.

---

## What “the Game” Means on the Server

On the server, a *game* is an **authoritative simulation instance** running
inside a room.

A game is defined by:
- a world state (entities, components, systems)
- a fixed simulation timeline
- a set of connected, authenticated players
- deterministic rules enforced server-side

The server never renders the game. It only computes state.

---

## Authority Model

The server is the **single source of truth** for:
- entity creation and destruction
- positions, collisions, damage, deaths
- scoring and win/lose conditions
- game phase transitions

Clients:
- send *inputs*
- receive *snapshots*
- may predict or smooth locally
- must always converge to server state

Any discrepancy is resolved in favor of the server.

---

## Game Lifecycle

A game instance follows a strict lifecycle:

1. **Lobby / Waiting**
    - players join a room
    - no simulation is running yet
    - server waits for start conditions

2. **Initialization**
    - world is created
    - entities are spawned (players, initial enemies, environment)
    - snapshot sequence is reset
    - game state transitions to running

3. **Running**
    - fixed-tick simulation loop is active
    - inputs are processed
    - snapshots are produced and sent
    - win/lose conditions are evaluated

4. **Game End**
    - final state is resolved
    - results are computed
    - clients are notified
    - room returns to lobby or is destroyed

Transitions are server-driven and occur at tick boundaries.

---

## World and Simulation

Each game owns a **World**:
- entity registry
- component storage
- system pipeline

Systems are executed in a stable, predefined order each tick.
No system may depend on client timing or rendering concerns.

Typical system categories:
- movement and physics
- collisions and combat
- AI and spawns
- cleanup and lifetime management
- scoring and progression

---

## Player Representation

Each connected player is represented by:
- an authenticated session
- a player entity in the world
- an input queue

Rules:
- inputs are validated and rate-limited
- missing inputs imply “no action”
- disconnected players are removed or marked inactive

The server never trusts client-side position or state claims.

---

## Networking Integration

### Input
- received over UDP
- associated with a bound session
- queued per room
- applied at tick boundaries

### Output
- authoritative snapshots
- sent over UDP
- include sequence numbers
- may be rate-limited or dropped under load

Critical transitions (start/end, kicks, errors) are sent over TCP.

---

## Determinism and Fairness

While perfect determinism across machines is not required, the server enforces:

- fixed timestep simulation
- stable system ordering
- bounded input processing
- consistent rules for all players

This ensures fairness and predictability.

---

## Failure and Isolation

- A client failure must not stop the game loop
- A single room failure should not crash the server
- The runtime must isolate rooms whenever possible

If a game becomes unrecoverable:
- end the game cleanly
- notify players
- release resources

---

## Non-goals

- The server does not handle rendering or UI
- The server does not attempt to correct client visuals
- The server does not guarantee delivery of UDP snapshots
- The server does not trust client-side simulation

---

## Invariants

- The server owns the game timeline
- All gameplay state is authoritative server-side
- Clients influence the game only via validated inputs
- Simulation correctness has priority over delivery guarantees
