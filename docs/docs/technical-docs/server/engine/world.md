---
id: world
title: Game World 
sidebar_label: World
---

## Overview

The `IGameWorld` interface defines the minimal surface required for any game world implementation:

```cpp
virtual Ecs::Registry &registry() = 0;
virtual Ecs::Entity createPlayer() = 0;
virtual void destroyEntity(Ecs::Entity ent) = 0;
````

The concrete implementation `World` encapsulates:

* A single ECS registry
* Logic for entity creation/destruction
* Component registration

---

## ECS Registry Ownership

The world owns the ECS registry:

```cpp
Ecs::Registry _registry;
```

This ensures:

* The registry cannot be accessed outside controlled API
* Systems and GameServer operate consistently on the same state
* The world can enforce initialization rules and component constraints

---

## Component Initialization

During construction, the world registers all components required by the simulation:

```cpp
_registry.registerComponent<Id>();
_registry.registerComponent<Position>();
_registry.registerComponent<Velocity>();
_registry.registerComponent<Direction>();
_registry.registerComponent<Lifetime>();
_registry.registerComponent<Health>();
_registry.registerComponent<Damage>();
_registry.registerComponent<Damageable>();
_registry.registerComponent<Attack>();
_registry.registerComponent<Projectile>();
_registry.registerComponent<HomingProjectile>();
_registry.registerComponent<WeaponConfig>();
_registry.registerComponent<AIBrain>();
_registry.registerComponent<AIShoot>();
_registry.registerComponent<MovementPattern>();
_registry.registerComponent<Target>();
_registry.registerComponent<Controllable>();
_registry.registerComponent<PowerUp>();
_registry.registerComponent<PowerUpType>();
_registry.registerComponent<PlayerPowerUp>();
_registry.registerComponent<BubblePowerUp>();
_registry.registerComponent<LaserPowerUp>();
_registry.registerComponent<PowerUpBar>();
_registry.registerComponent<BossPart>();
_registry.registerComponent<BossPhase>();
_registry.registerComponent<TailFollower>();
_registry.registerComponent<Collision>();
_registry.registerComponent<PixelCollision>();
_registry.registerComponent<GravityAffected>();
_registry.registerComponent<GravityField>();
_registry.registerComponent<Drawable>();
_registry.registerComponent<KillScore>();
_registry.registerComponent<Score>();
_registry.registerComponent<InputComponent>();
_registry.registerComponent<Background>();
```

This centralizes component lifecycle management and guarantees availability.

---

## Entity Creation

`createPlayer()` constructs a new game-ready entity:

```cpp
auto ent = _registry.createEntity();
_registry.emplaceComponent<Position>(ent, 100.f, 100.f);
_registry.emplaceComponent<Velocity>(ent, 0.f, 0.f);
_registry.emplaceComponent<Health>(ent, 100, 100);
_registry.emplaceComponent<InputComponent>(ent);
```

This ensures that every player is born with:

* a position
* velocity
* health
* input state

No partially-initialized entities can exist.

---

## Entity Destruction

`destroyEntity()` removes an entity and all attached components:

```cpp
_registry.destroyEntity(ent);
```

This guarantees memory safety and prevents invalid component reads.

---

## Why IGameWorld?

Using an interface has three purposes:

1. **Abstraction**
   Systems and GameServer depend only on the world’s *capabilities*, not its concrete type.

2. **Testability**
   A `MockWorld` can be introduced for CI, a requirement for robust backend engines.

3. **Extensibility**
   Future worlds may include:

    * multiple layers
    * multi-room simulations
    * spatial partitioning
    * rollback worlds for netcode

IGameWorld is therefore a future-proof architectural decision.