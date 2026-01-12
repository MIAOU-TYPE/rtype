---
title: Overview
---

# Client Overview

This section documents the **Client** side of the R-Type project. It covers the
runtime, rendering, audio, UI, resources, networking, and authentication layers,
and explains how they interact as a coherent system.

The goal is to describe:
- how the client code is structured
- how data flows between subsystems
- which contracts (network, auth, events) the client must respect

This is architectural documentation, not an API reference.

---

## Client Role

The client is a **reactive, non-authoritative** application.

It is responsible for:
- presenting the game visually and audibly
- collecting user input
- communicating intent to the server
- reflecting server-authoritative state in a smooth and responsive way

The client never decides gameplay outcomes.

---

## High-Level Responsibilities

### Rendering
- Render the current game world and UI every frame
- Consume immutable, render-ready data
- Never mutate gameplay or network state

### Input and UI
- Collect user input (keyboard, mouse, UI actions)
- Convert input into high-level intent events
- Reflect runtime and network feedback (errors, state changes)

### State Synchronization
- Receive authoritative snapshots from the server
- Buffer, interpolate, and apply state deterministically
- Tolerate packet loss and reordering

### Authentication and Session
- Handle login and registration flows
- Maintain authenticated session state
- Manage token usage and optional persistence

### Resource Management
- Load textures, audio, fonts, and levels
- Enforce predictable asset layout
- Cache and validate resources

---

## Data Flow Summary

1. User input → UI events
2. UI events → runtime intent handling
3. Runtime → TCP/UDP network requests
4. Server responses → network events
5. Network events → runtime state updates
6. Runtime builds render data
7. Graphics/audio consume render data

All cross-system communication flows through events.

---

## Folder Map

The client codebase is organized by responsibility:

- `architecture/`  
  Main runtime loop, lifecycle management, threading

- `network/`  
  TCP and UDP channels, protocol handling, state synchronization

- `auth/`  
  Authentication flows, token handling, persistence, UI wiring

- `ui/`  
  Menus, navigation, HUD, view models

- `graphics/`  
  Rendering architecture, sprite usage, debug overlays

- `audio/`  
  Sound and music playback, priorities, volume groups

- `resources/`  
  Asset loading, caching, validation, level data

- `events/`  
  Event bus, event categories, UI ↔ runtime ↔ network contracts

This separation enforces clear ownership boundaries.

---

## Architectural Contracts

The client must respect the following contracts:

- The server is the sole authority for:
    - entity state
    - room membership
    - game timeline
- The client may predict or smooth visuals, but must always converge to server state
- Networking, UI, rendering, and audio remain decoupled
- All long-running or blocking operations are isolated from the render loop

---

## Invariants

- The client never trusts itself for authoritative gameplay state
- The server is the source of truth
- Responsiveness is prioritized without violating correctness
- Presentation layers (graphics/audio/UI) never drive game logic
