---
title: Levels
---

# Levels

Levels are treated as **data-driven descriptions** rather than hard-coded logic.
They define the structure and presentation of a game stage while leaving
authoritative gameplay decisions to the server.

---

## Level Content

A level description may include:

- Background configuration:
    - parallax layers
    - scrolling speed
    - visual themes
- Spawn definitions or wave scripts  
  (interpreted authoritatively by the server)
- Visual-only metadata:
    - lighting or color grading
    - ambience settings
    - music track identifier

Level data should be declarative and serializable.

---

## Authority Model

- The server is authoritative for:
    - enemy spawns
    - timing
    - progression rules
- The client never generates spawns or gameplay outcomes on its own

The client only reflects server-driven state.

---

## Client Responsibilities

On the client side:
- Load the appropriate level assets
- Configure background rendering and ambience
- Play the level’s music track
- Apply visual cues based on level metadata

All gameplay entities originate from server snapshots.

---

## Level Transitions

When switching levels:
- Clear client-side entities
- Reset interpolation and state sync buffers
- Load new background and ambience assets
- Await server confirmation before rendering gameplay entities

---

## Extensibility Guidelines

- New levels should be addable without recompiling the client
- Visual-only changes must not affect gameplay correctness
- Level formats should be versioned to allow evolution

---

## Invariants

- Level data is treated as read-only
- Client rendering never drives gameplay
- Server state always overrides local assumptions
- Missing level metadata must not crash the client

