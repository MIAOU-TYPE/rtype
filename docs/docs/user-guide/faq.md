---
title: FAQ
---

# FAQ

This page answers **frequently asked questions** about running and using the
R-Type project. The focus is on practical understanding rather than
implementation details.

---

## Why does the server decide everything?

The server is **authoritative by design**. This prevents:
- cheating
- desynchronization between players
- divergent timelines

Clients are responsible for responsiveness and presentation, but all outcomes
(collisions, damage, scoring, game start/end) are decided server-side.

---

## Why does the game freeze after login?

Most commonly, this means **UDP traffic is not flowing**.

Typical causes:
- UDP blocked by firewall or NAT
- missing or failed UDP bind step
- client and server using different UDP ports

TCP may still work (login, lobby), but gameplay requires UDP.

---

## Why do I need both TCP and UDP?

They solve different problems:

- **TCP**
    - reliable and ordered
    - used for auth, rooms, and critical confirmations

- **UDP**
    - low latency
    - used for real-time inputs and snapshots

Using only TCP would add latency; using only UDP would break correctness.

---

## Why don’t inputs always apply immediately?

Inputs:
- are sent over UDP
- may be dropped or delayed
- are validated server-side

The client never applies inputs authoritatively. What you see is the server’s
approved state, not immediate local prediction.

---

## Why do entities sometimes snap or teleport?

This usually indicates:
- packet loss
- large time gaps between snapshots
- interpolation buffer reset (join, room switch)

The client prefers correctness over smoothness. Large corrections are expected
in degraded network conditions.

---

## Why can I create a room but not start the game?

Game start is **server-controlled**.

Possible reasons:
- not enough players
- missing permissions (host-only start)
- server-side game state not ready

The UI waits for a server confirmation before transitioning.

---

## Why am I returned to the login screen?

Common causes:
- session expired
- invalid or stale auth token
- server restart
- TCP disconnect

When this happens, the client clears session state and requires re-authentication.

---

## Can I reconnect to a game in progress?

This depends on the implementation.

In many setups:
- reconnecting mid-game is **not supported**
- clients must rejoin after returning to the lobby

This is a design choice, not a bug.

---

## Why does the server keep running when no players are connected?

The server:
- owns the simulation
- manages rooms and lifecycle
- is designed to be long-running

Clients are ephemeral; the server process is not.

---

## Is lag always a network problem?

Not always.

Other causes include:
- server overload
- too many clients on one machine
- excessive debug logging
- very high snapshot or tick rates

Network latency is only one factor.

---

## Where should I look if something breaks?

Start by identifying the category:
- **Auth / TCP**: login, lobby, room actions
- **UDP**: gameplay, movement, snapshots
- **Client UI**: menus, transitions
- **Server simulation**: rules, timing, authority

Then consult:
- *Tips*
- *Troubleshooting*
- logs on both client and server

---

## I still don’t understand what’s wrong. What now?

Ask:
1. What state am I in (login, lobby, game)?
2. What channel is involved (TCP or UDP)?
3. Who owns authority for this action?

Answering these three questions usually narrows the problem immediately.
