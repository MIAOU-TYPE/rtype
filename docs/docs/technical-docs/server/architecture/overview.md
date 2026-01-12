---
title: Overview
---

# Server Architecture Overview

This section documents the **Server** side architecture of the R-Type project.

The server is the **authoritative simulation**: it owns gameplay rules, entity lifecycle, room instances,
and the official timeline. It must remain deterministic, resilient to client failures, and safe against malformed
network traffic.

The goal of this documentation is to explain:
- how the server runtime is structured
- how subsystems interact (networking, rooms, ECS/systems, auth)
- what ordering and lifecycle rules must be respected

---

## Server Role

The server is responsible for:
- Running the authoritative game simulation (entities, collisions, spawns, scoring, rules)
- Hosting multiple rooms / instances (lobby + game sessions)
- Managing user identity and sessions (auth + UDP binding token)
- Broadcasting authoritative state to clients (snapshots over UDP)
- Processing client inputs (movement, shooting) and validating them

Clients are allowed to be responsive and predictive, but the server decides the truth.

---

## High-Level Responsibilities

### Runtime and lifecycle
- Start networking channels and worker threads
- Run tick-based simulation loops independent of client timing
- Monitor health, handle shutdown safely, and avoid leaks

### Room / instance management
- Create, list, join, and destroy rooms
- Isolate game state per room
- Enforce capacity limits and membership rules

### Simulation (ECS / Systems)
- Maintain entity registry and components
- Execute systems in a stable order each tick
- Produce snapshot data for replication to clients

### Networking
- TCP control plane:
    - auth
    - lobby operations (list/create/join/leave)
    - critical events and errors
- UDP data plane:
    - receive client inputs
    - send authoritative snapshots
    - tolerate packet loss and reordering

### Authentication and sessions
- Store accounts (database)
- Validate credentials
- Issue session tokens and short-lived UDP bind tokens
- Enforce expiration and invalidation rules

---

## Data Flow Summary

1. Client connects over TCP and authenticates
2. Server issues a session token and a UDP bind token (or equivalent)
3. Client binds its UDP endpoint
4. Client sends input commands over UDP
5. Server:
    - validates inputs
    - advances simulation at fixed ticks
    - emits snapshots over UDP
6. Server sends authoritative state transitions over TCP (room events, game start/end)

---

## Structure and Folder Map

The server documentation is organized by responsibility:

- `architecture/`  
  Runtime overview, main loop, threading, lifecycle, shutdown rules

- `network/`  
  TCP/UDP responsibilities, protocol rules, socket abstractions, reliability assumptions

- `auth/`  
  Database, password hashing, session management, UDP bind token issuance, TCP auth API

- `resources/`  
  Configuration and persistence concerns (server-side config, storage layout)

- `events/`  
  Event bus or internal messaging patterns used to decouple subsystems (if applicable)

- `engine/`  
  Simulation architecture: ECS, systems ordering, world state, game server integration

---

## Ordering and Invariants

### Authority
- The server is the source of truth for:
    - entity state
    - room membership
    - game rules and outcomes
    - the official timeline

### Robustness
- A misbehaving or disconnected client must not crash the server
- Malformed packets must be rejected safely (no excessive allocation, no undefined behavior)

### Determinism (practical)
- Simulation advances on fixed ticks
- System execution order is stable
- Replication output is derived from the authoritative state only

### Network safety
- UDP packets are ignored until a UDP endpoint is bound to an authenticated session
- TCP disconnects invalidate session-dependent state
- Rate limiting may be applied to bind attempts and auth failures

---

## Failure Modes

- **TCP disconnect**
    - drop control-plane state for that client
    - invalidate session and room membership as required

- **UDP silent loss**
    - continue simulation
    - clients will recover via later snapshots

- **Room crash / exception**
    - isolate failure to the room when possible
    - avoid bringing down the full server process

---

## Non-goals

- The server does not guarantee perfect reliability for UDP updates
- The server does not trust client-side prediction for authoritative decisions
- The server does not attempt to “fix” client rendering issues (only provides state)
