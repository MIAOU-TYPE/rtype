---
title: Multiplayer Flow
---

# Multiplayer Flow

This document describes the **end-to-end multiplayer flow** from a user and
runtime perspective. It explains how the client and server interact from startup
to gameplay, without going into low-level implementation details.

The goal is to make the multiplayer lifecycle easy to reason about.

---

## High-Level Overview

Multiplayer gameplay follows a **server-authoritative model**:

- the **server** controls simulation, rooms, and game state
- the **client** handles input, rendering, and UI
- networking is split into:
  - TCP for control and guarantees
  - UDP for real-time gameplay data

At no point does the client become authoritative.

---

## Global Flow

```text
Client                                Server
  │                                     │
  │ Start client                        │
  │------------------------------------>│
  │                                     │
  │ TCP connect                         │
  │------------------------------------>│
  │                                     │
  │ Login / Register (TCP)              │
  │------------------------------------>│
  │                                     │
  │ Auth OK + session (TCP)             │
  │<------------------------------------│
  │                                     │
  │ Receive UDP bind token              │
  │                                     │
  │ UDP bind (token)                    │
  │------------------------------------>│
  │                                     │
  │ Enter lobby                         │
  │                                     │
  │ List / create / join room (TCP)     │
  │------------------------------------>│
  │                                     │
  │ Room confirmed (TCP)                │
  │<------------------------------------│
  │                                     │
  │ Wait for game start                 │
  │                                     │
  │ Game start (TCP event)              │
  │<------------------------------------│
  │                                     │
  │ Gameplay loop (UDP inputs/snapshots)│
  │<===================================>│
````
## Detailed Flow

### 1. Server Startup

* Server loads configuration and persistence
* TCP and UDP sockets are opened
* Simulation loop starts
* Server is ready to accept clients

No clients are required for the server to run.

---

### 2. Client Startup

* Client initializes graphics, audio, and input
* Client opens TCP connection to the server
* No UDP traffic is accepted yet

---

### 3. Authentication (TCP)

* Client sends `LOGIN` or `REGISTER` request over TCP
* Server validates credentials
* On success:

    * server creates a session
    * server returns auth success and session metadata
* On failure:

    * client stays on login screen with error

At this stage:

* client is authenticated
* no gameplay traffic is allowed yet

---

### 4. UDP Bind (Anti-Spoofing)

Because UDP is connectionless:

1. Server issues a short-lived **UDP bind token**
2. Client sends a `UDP_BIND` packet containing the token
3. Server validates:

    * token validity
    * expiration
    * single-use or renewal rules
4. Server associates `(ip, port)` with the session

After binding:

* UDP packets from unbound addresses are rejected
* gameplay traffic is allowed

---

### 5. Lobby Phase (TCP)

Client enters the lobby and can:

* list available rooms
* create a room
* join an existing room

All lobby actions:

* go through TCP
* require server confirmation
* update UI only after confirmation

The server enforces:

* room capacity
* permissions
* room lifecycle rules

---

### 6. Room Phase

Once inside a room:

* client waits for the server to start the game
* no simulation runs client-side
* room state changes are driven by server events

The server decides when the game starts.

---

### 7. Game Start

* Server transitions the room to *in-game*
* Server sends a `GAME_START` event over TCP
* Clients switch to in-game mode

This is the synchronization point between all players.

---

### 8. Gameplay Loop (UDP)

During gameplay:

#### Client → Server

* player inputs are sent over UDP
* inputs are:

    * best-effort
    * sequence-tagged
    * rate-limited server-side

#### Server → Client

* server runs the authoritative simulation
* server produces snapshots
* snapshots are sent over UDP at a fixed rate

The client:

* buffers snapshots
* interpolates state
* renders visuals and audio

---

### 9. Game End

When the game ends:

* server determines final state
* server sends final events/snapshots
* room transitions back to lobby or is destroyed

Clients update UI accordingly.

---

### 10. Disconnects and Errors

* TCP disconnect:

    * session is invalidated
    * client returns to main menu
* UDP silence:

    * client may show “connection degraded”
    * TCP remains authoritative
* Auth errors:

    * client is redirected to login
    * persisted tokens are cleared

---

## Key Guarantees

* Server is always authoritative
* Clients never simulate independently
* TCP controls structure and intent
* UDP carries time-sensitive data
* State transitions happen only on server confirmation

---

## Mental Model

If something goes wrong, ask:

1. Is this a **TCP control problem** (auth, room, confirmation)?
2. Is this a **UDP data problem** (bind, snapshots, inputs)?
3. Who owns authority for this state?

The answer usually points directly to the issue.
