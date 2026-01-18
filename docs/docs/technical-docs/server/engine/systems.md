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
- `MovementPatternSystem`
- `CollisionSystem`
- `ShootingSystem`
- `PowerUpShootingSystem`
- `AIShootSystem`
- `HomingSystem`
- `GravitySystem`
- `LifetimeSystem`
- `BossSystem`
- `TailFollowerSystem`
- `HealthSystem`
- `PowerUpAttachmentSystem`
- `PowerUpBarSystem`
- `BubblePowerUpSystem`
- `LaserPowerUpSystem`
- `BackgroundSystem`
- `SnapshotSystem`

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
MovementPatternSystem::update(world, dt);
ShootingSystem::update(world, dt);
PowerUpShootingSystem::update(world, dt);
AIShootSystem::update(world, dt);
HomingSystem::update(world, dt);
GravitySystem::update(world, dt);
CollisionSystem::update(world);
HealthSystem::update(world);
LifetimeSystem::update(world);
BossSystem::update(world, dt);
TailFollowerSystem::update(world, dt);
PowerUpAttachmentSystem::update(world);
PowerUpBarSystem::update(world, dt);
BubblePowerUpSystem::update(world, dt);
LaserPowerUpSystem::update(world, dt);
BackgroundSystem::update(world, dt);
SnapshotSystem::update(world);
```

This order ensures:

1. Input modifies velocity
2. Movement uses updated velocity to modify positions
3. Complex movements are applied
4. Projectiles are created and updated
5. AI decisions are made
6. Homing projectiles track targets
7. Gravity affects entities
8. Collisions are detected and resolved
9. Damage is applied and entities die
10. Temporary entities expire
11. Boss logic and tail following
12. Power-ups are collected and managed
13. Visual updates for backgrounds
14. Snapshots are prepared for networking

## Detailed System Descriptions

### InputSystem
Processes player input from `InputComponent` and updates `Velocity` components. Converts keyboard/gamepad input into movement intent.

### MovementSystem
Applies velocity to position over time: `pos.x += vel.vx * dt; pos.y += vel.vy * dt;`

### MovementPatternSystem
Handles complex enemy movement patterns like sinusoidal waves, circular paths, and scripted behaviors defined in `MovementPattern` components.

### CollisionSystem
Detects collisions between entities using `Collision` components. Applies damage and triggers effects when entities overlap.

### ShootingSystem
Manages weapon firing, cooldowns, and projectile creation for basic weapons.

### PowerUpShootingSystem
Handles special power-up weapons (laser, bubble) with unique firing mechanics and charge management.

### AIShootSystem
Controls enemy shooting patterns and timing based on `AIShoot` components.

### HomingSystem
Updates homing projectile trajectories to track targets specified in `HomingProjectile` components.

### GravitySystem
Applies gravitational forces from `GravityField` entities to `GravityAffected` entities.

### LifetimeSystem
Removes entities when their `Lifetime` expires.

### BossSystem
Manages boss behaviors, phase transitions, and multi-part logic.

### TailFollowerSystem
Updates boss tail segments to follow the main body with physics-based following.

### HealthSystem
Applies damage from `Damage` components and handles entity destruction when health reaches zero.

### PowerUpAttachmentSystem
Manages power-up collection and attachment to player entities.

### PowerUpBarSystem
Updates power-up charge bars and UI state.

### BubblePowerUpSystem
Handles bubble shield mechanics and collision immunity.

### LaserPowerUpSystem
Manages laser weapon charging and firing.

### BackgroundSystem
Updates scrolling background layers.

### SnapshotSystem
Prepares world state snapshots for network synchronization.

---

## Stateless Design

Systems must not store internal state because:

* State belongs to the world, not to the logic
* Systems should remain reusable and deterministic
* Parallelization becomes possible later

This aligns with standard ECS architecture principles used in modern game engines.
