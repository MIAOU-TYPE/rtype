---
id: systems
title: Gameplay Systems
sidebar_label: Systems
---

## Overview

A gameplay system is a stateless unit of logic that transforms components in the ECS registry.  
A system never stores state of its own; instead, it operates on the world’s components.

Current implemented systems:

- `InputSystem`
- `MovementSystem`

---

## Why Systems Receive `World&`

Systems do not own the world and should not copy it.  
They modify live ECS components directly.

Example:

```cpp
void InputSystem::update(World& world);
void MovementSystem::update(World& world, float dt);
````

Reasons for using references:

1. They must operate on the *actual* world, not a copy
2. They cannot operate on null (so pointers are unsafe)
3. Passing by reference enforces correctness at compile time

---

## InputSystem

The input system reads `InputComponent` and writes into `Velocity`:

```cpp
vel.vx = input.right ? +speed : input.left ? -speed : 0.f;
vel.vy = input.down  ? +speed : input.up   ? -speed : 0.f;
```

Input does not move the entity — it only updates the intent.

---

## MovementSystem

Movement applies velocity to position:

```cpp
pos.x += vel.vx * dt;
pos.y += vel.vy * dt;
```

Movement must be called *after* input to ensure simulation consistency.

---

## System Execution Order

Current update loop in `GameServer`:

```cpp
InputSystem::update(world);
MovementSystem::update(world, dt);
```

This order ensures:

1. Input modifies velocity
2. Movement uses updated velocity to modify positions

Future systems might include:

* Collision resolution
* AI steering
* Projectile updates
* Damage processing
* Snapshot generation for the network

These must also follow deterministic ordering rules for multiplayer synchronization.

---

## Stateless Design

Systems must not store internal state because:

* State belongs to the world, not to the logic
* Systems should remain reusable and deterministic
* Parallelization becomes possible later

This aligns with standard ECS architecture principles used in modern game engines.
