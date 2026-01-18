---
id: engine
title: Engine Details
sidebar_label: Engine Details
---

# R-TYPE Engine Details

This document provides detailed information about the Entity Component System (ECS) components and systems implemented in the R-TYPE server engine.

## ECS Components

The ECS architecture uses components to define entity properties and behaviors. Below are the key components implemented:

### Core Components

- **Id**: Unique identifier for entities
- **Position**: 2D position (x, y) in the game world
- **Velocity**: Movement velocity (dx, dy)
- **Direction**: Facing direction vector
- **Lifetime**: Time-to-live for temporary entities

### Combat Components

- **Health**: Current and maximum hit points
- **Damage**: Damage value for projectiles
- **Damageable**: Marks entities that can take damage
- **Attack**: Attack properties and cooldowns
- **Projectile**: Projectile-specific properties (speed, damage, type)
- **HomingProjectile**: Homing behavior (target ID, strength, max speed)

### Enemy AI Components

- **AIBrain**: AI behavior state and decision-making
- **AIShoot**: AI shooting patterns and timing
- **MovementPattern**: Complex movement behaviors (sinusoidal, circular, etc.)
- **Target**: Target tracking for homing projectiles

### Power-Up Components

- **PowerUp**: Marks collectible power-up entities
- **PowerUpType**: Defines power-up categories (force, laser, bubble)
- **PlayerPowerUp**: Active power-up state on players
- **BubblePowerUp**: Bubble shield mechanics
- **LaserPowerUp**: Laser weapon mechanics

### Boss Components

- **BossPart**: Multi-part boss hitboxes with damage multipliers
- **BossPhase**: Boss phase transitions and behaviors
- **TailFollower**: Boss tail segment following logic

### Physics Components

- **Collision**: Collision detection properties
- **PixelCollision**: Pixel-perfect collision data
- **GravityAffected**: Entities affected by gravity fields
- **GravityField**: Gravity source entities
- **Controllable**: Player-controlled entities

### Rendering Components

- **Drawable**: Rendering properties (sprite ID, layer)

### Scoring Components

- **KillScore**: Points awarded for destroying enemies
- **Score**: Player score tracking

### Input Components

- **InputComponent**: Player input state

### Background Components

- **Background**: Background layer properties

## ECS Systems

Systems process entities with specific component combinations. Each system runs in order during the game loop.

### Movement Systems

- **MovementSystem**: Applies velocity to positions using delta-time. Updates Position components based on Velocity components.

- **MovementPatternSystem**: Handles complex enemy movement patterns (sinusoidal waves, circular paths, etc.).

### Combat Systems

- **CollisionSystem**: Detects collisions between entities and applies damage. Handles projectile-enemy and projectile-player interactions.

- **ShootingSystem**: Manages weapon firing, cooldowns, and projectile creation.

- **PowerUpShootingSystem**: Handles special power-up weapons (laser, bubble) with unique firing mechanics.

- **AIShootSystem**: Controls enemy shooting patterns and timing.

- **HomingSystem**: Updates homing projectile trajectories toward targets.

### Physics Systems

- **GravitySystem**: Applies gravitational forces to affected entities.

- **LifetimeSystem**: Removes entities when their lifetime expires.

### Boss Systems

- **BossSystem**: Manages boss behaviors, phase transitions, and multi-part logic.

- **TailFollowerSystem**: Updates boss tail segments to follow the main body.

### Power-Up Systems

- **PowerUpAttachmentSystem**: Manages power-up collection and attachment to players.

- **PowerUpBarSystem**: Updates power-up charge bars and UI.

- **BubblePowerUpSystem**: Handles bubble shield mechanics and collision immunity.

- **LaserPowerUpSystem**: Manages laser weapon charging and firing.

### Utility Systems

- **InputSystem**: Processes player input and updates controllable entities.

- **HealthSystem**: Manages damage application and entity destruction.

- **SnapshotSystem**: Prepares world state for network synchronization.

- **BackgroundSystem**: Updates scrolling background layers.

## System Execution Order

Systems are executed in a specific order to ensure correct dependencies:

1. InputSystem (process player input)
2. MovementSystem (apply velocities)
3. MovementPatternSystem (complex movements)
4. ShootingSystem (create projectiles)
5. PowerUpShootingSystem (special weapons)
6. AIShootSystem (AI decisions)
7. HomingSystem (update projectile paths)
8. GravitySystem (apply gravity)
9. CollisionSystem (detect and resolve collisions)
10. HealthSystem (apply damage and destroy entities)
11. LifetimeSystem (remove expired entities)
12. BossSystem (boss-specific logic)
13. TailFollowerSystem (boss tail updates)
14. PowerUpAttachmentSystem (power-up collection)
15. PowerUpBarSystem (UI updates)
16. BubblePowerUpSystem (shield mechanics)
17. LaserPowerUpSystem (laser mechanics)
18. BackgroundSystem (visual updates)
19. SnapshotSystem (network prep)