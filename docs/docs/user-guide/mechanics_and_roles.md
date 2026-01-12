---
title: Mechanics and Roles
---

# Mechanics and Roles

This page describes the **conceptual model** of the R-Type project: the division of
responsibilities between server and client, how sessions and rooms work, and how
networking and state synchronization are handled at a high level.

The goal is to make clear **who decides what**, and **where authority lives**.

---

## Roles

### Server (Authoritative)

The server is the **single source of truth**. It is responsible for:

- Owning the official game timeline (fixed-tick simulation)
- Validating and rate-limiting all client inputs
- Running the simulation (ECS, systems, world rules)
- Resolving collisions, damage, scoring, and win conditions
- Managing rooms, players, and lifecycle transitions
- Producing authoritative snapshots of the world state
- Replicating snapshots to clients over UDP

The server:
- never trusts client-reported state
- never depends on client frame rate or timing
- decides all outcomes

If there is a disagreement between client and server, the server always wins.

---

### Client (Presentation + Input)

The client is responsible for **responsiveness and presentation**, not authority.

It:
- Renders the world, UI, and effects
- Plays audio (music and sound effects)
- Collects player input
- Sends input commands to the server
- Receives authoritative snapshots from the server
- Interpolates state for smooth rendering
- Handles authentication, menus, and navigation

The client:
- does not run the authoritative simulation
- does not decide outcomes
- never trusts its own predicted state over the server

Client-side prediction or smoothing (if present) is strictly visual.

---

## Rooms and Sessions

A typical multiplayer session follows this sequence:

1. **Authenticate**
    - Client logs in or registers over TCP
    - Server creates a session

2. **Enter the lobby**
    - Client lists available rooms
    - Server tracks room metadata and membership

3. **Create or join a room**
    - Requests are sent over TCP
    - Server validates capacity and permissions

4. **Wait for game start**
    - Game start is server-controlled
    - Clients remain idle until the server transitions the room

5. **Play**
    - Simulation runs on the server
    - Inputs flow from clients to server
    - Snapshots flow from server to clients

6. **Exit**
    - On game end: return to lobby
    - On disconnect or error: return to main menu

Room membership and lifecycle are always enforced by the server.

---

## Networking Model

The networking stack is split into two channels with distinct roles:

### TCP — Control Plane
Used for:
- authentication (login, register, logout)
- room creation and joins
- critical acknowledgements
- error reporting

TCP guarantees delivery and ordering, making it suitable for **low-frequency,
high-importance** messages.

---

### UDP — Data Plane
Used for:
- player input commands
- real-time world snapshots
- transient gameplay updates

UDP favors low latency over reliability. The system assumes:
- packet loss is normal
- packets may arrive out of order
- newer data supersedes older data

---

### UDP Bind (Anti-Spoofing)

Because UDP is connectionless, a bind step is required:

1. Client authenticates via TCP
2. Server issues a short-lived UDP bind token
3. Client sends a UDP bind packet containing the token
4. Server associates `(ip, port)` with the authenticated session

After binding:
- only packets from the bound endpoint are accepted
- NAT rebinding requires a new bind
- stolen or expired tokens are rejected

---

## State Synchronization (High Level)

The server continuously produces authoritative snapshots:

- Each snapshot has a monotonically increasing `sequence`
- Snapshots represent server-approved world state

On the client:
- snapshots are buffered
- rendering happens at `now - interpolationDelay`
- entity transforms are interpolated between snapshots
- missing packets cause brief freezes or extrapolation, not corruption

Rules:
- newer snapshots always override older ones
- packet loss is tolerated
- visual smoothness must never override correctness

---

## Key Invariants

- The server is always authoritative
- The client is never trusted for game state
- TCP controls intent and structure
- UDP carries time-sensitive data
- Rooms and sessions are server-owned
- Visual smoothness must not break correctness
