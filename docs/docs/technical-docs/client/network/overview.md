---
title: Networking Overview
sidebar_label: Overview
---

# Networking Overview

The client networking layer is split into **two complementary channels** with
clear and non-overlapping responsibilities. This separation is fundamental to
achieving both responsiveness and correctness.

---

## Channel Roles

### TCP — Control Plane
TCP is used for **reliable, ordered, state-changing operations**.

Typical usage:
- authentication and session management
- matchmaking and room lifecycle
- game start / end signals
- critical acknowledgements and errors
- protocol handshake and keep-alive

TCP traffic is low-frequency and must be deterministic.

---

### UDP — Data Plane
UDP is used for **real-time, high-frequency data**.

Typical usage:
- world state snapshots
- fast input commands
- transient gameplay updates

UDP prioritizes timeliness over reliability. Late or missing packets are
discarded rather than retransmitted.

---

## Design Goals

### Responsiveness
- Minimize end-to-end latency for state updates
- Prefer dropping stale data over delaying new data
- Allow client-side smoothing (interpolation)

### Correctness
- All authoritative decisions are confirmed over TCP
- UDP data is never trusted without prior authentication and binding

### Stability
- Tolerate packet loss, duplication, and reordering
- Prevent malformed or spoofed packets from impacting the client

---

## Network Data Flow

1. Client authenticates over TCP
2. UDP endpoint is bound to the authenticated session
3. TCP controls high-level state transitions
4. UDP streams real-time state updates
5. Runtime merges both streams into a coherent client state

---

## Documents in This Section

- `tcp`  
  TCP message scope, request/response patterns, keep-alive behavior

- `udp`  
  UDP rules, binding mechanism, anti-spoofing assumptions

- `state-sync`  
  Snapshot buffering, interpolation, and application to client state

Each document focuses on a single concern to keep complexity isolated.

---

## Failure Modes and Client Behavior

### TCP Disconnect
- Clear authentication and session state
- Stop all gameplay-related activity
- Return to main menu

### UDP Silent Drop
- Keep rendering the last valid state
- Mark connection as degraded
- Attempt rebind or recovery in background

### Authentication Error
- Clear in-memory and persisted tokens
- Redirect to login screen
- Require explicit user action

---

## Invariants

- TCP and UDP responsibilities never overlap
- Gameplay never depends on TCP latency
- UDP data is ignored until explicitly bound
- Network failures never crash the client

---
