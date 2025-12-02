---
id: physics
title: Physics
sidebar_label: Physics
---

# Physics System

The physics module handles movement, collisions, and interactions.

## Features

- Axis-aligned bounding box (AABB) collisions.
- Velocity-based movement.
- Gravity and forces (if needed).
- Collision events triggering damage or effects.

## Example

```cpp
CollisionSystem::update(ecs);
if (CollisionSystem::check(player, enemy)) {
    HealthSystem::applyDamage(player, 1);
}
```