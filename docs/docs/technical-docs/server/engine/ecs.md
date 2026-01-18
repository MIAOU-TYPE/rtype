---
id: ecs
title: Entity Component System
sidebar_label: ECS
---

# Entity Component System (ECS)

The **Entity Component System (ECS)** is the core architecture of the R-TYPE engine.  
It allows the game to manage thousands of entities efficiently while keeping code modular, flexible, and maintainable.  

In ECS, data and logic are **separated**:
- **Entities** are unique identifiers (IDs) that represent objects in the game.
- **Components** store **data only** (position, health, velocity, etc.).
- **Systems** contain **logic** that processes entities with specific components.

---

## Entities

An **entity** is simply a unique ID. It does **not store any data** itself.  

- Think of an entity as a "tag" or "handle" for a game object.
- Entities are created by the **Registry** and can be destroyed at any time.
- Components are attached to entities to define their behavior and properties.

For example:
- A player entity may have `Position`, `Velocity`, `Health`, and `Drawable`.
- An enemy entity may have `Position`, `Velocity`, `AI_Brain`, `Health`, and `Attack`.

---

## Components

**Components are pure data** structures.  
Each component represents one aspect of an entity.  

### Complete List of Components in R-TYPE ECS

#### Core Components
- **Id**: Unique entity identifier
- **Position**: 2D position (x, y) coordinates
- **Velocity**: Movement velocity (dx, dy)
- **Direction**: Facing direction vector
- **Lifetime**: Time-to-live for temporary entities

#### Combat Components
- **Health**: Current and maximum hit points
- **Damage**: Damage value for projectiles
- **Damageable**: Marks entities that can take damage
- **Attack**: Attack properties and cooldowns
- **Projectile**: Projectile-specific properties (speed, damage, type)
- **HomingProjectile**: Homing behavior (target ID, strength, max speed)
- **WeaponConfig**: Weapon configuration and upgrades

#### Enemy AI Components
- **AIBrain**: AI behavior state and decision-making
- **AIShoot**: AI shooting patterns and timing
- **MovementPattern**: Complex movement behaviors (sinusoidal, circular, etc.)
- **Target**: Target tracking for homing projectiles
- **Controllable**: Player-controlled entities

#### Power-Up Components
- **PowerUp**: Marks collectible power-up entities
- **PowerUpType**: Defines power-up categories (force, laser, bubble)
- **PlayerPowerUp**: Active power-up state on players
- **BubblePowerUp**: Bubble shield mechanics
- **LaserPowerUp**: Laser weapon mechanics
- **PowerUpBar**: Power-up charge visualization

#### Boss Components
- **BossPart**: Multi-part boss hitboxes with damage multipliers
- **BossPhase**: Boss phase transitions and behaviors
- **TailFollower**: Boss tail segment following logic

#### Physics Components
- **Collision**: Collision detection properties
- **PixelCollision**: Pixel-perfect collision data
- **GravityAffected**: Entities affected by gravity fields
- **GravityField**: Gravity source entities

#### Rendering Components
- **Drawable**: Rendering properties (sprite ID, layer)

#### Scoring Components
- **KillScore**: Points awarded for destroying enemies
- **Score**: Player score tracking

#### Input Components
- **InputComponent**: Player input state

#### Background Components
- **Background**: Background layer properties
- **Score**: Tracks points gained from actions.

**Key points:**
- Components do not know about other components.
- They are lightweight and can be attached or removed dynamically.

---

## SparseArray

The ECS stores components in **SparseArrays**, which are **indexed by entity ID**.  

- Each slot in the array corresponds to an entity ID.
- A slot can contain a component or be empty (using `std::optional` in C++).
- This allows **fast access** to components by entity.
- Systems iterate over these arrays to process entities efficiently.

**Example conceptually:**
- `PositionSparseArray[entityID]` → position data for that entity.
- `VelocitySparseArray[entityID]` → velocity data.
- If an entity does not have a component, its slot is empty.

Benefits:
- Fast iteration over active components.
- Minimal memory usage for sparse entities.
- Easy to add/remove components at runtime.

---

## Registry

The **Registry** is the **central manager** of ECS:

- Creates and destroys entities.
- Registers component types.
- Stores components in SparseArrays.
- Attaches and removes components from entities.
- Provides iteration over entities that have specific components.

**Workflow for developers:**
1. **Register component types** in the registry.
2. **Create entities** and attach components.
3. **Run systems** to process entities each frame.

> The Registry ensures entities and components remain **loosely coupled**, allowing modular game logic.

---

## Systems

**Systems** define the **behavior** of entities:

- They iterate only over entities that have the required components.
- Example systems in R-TYPE:
  - **MovementSystem**: Updates position using velocity.
  - **RenderSystem**: Draws all entities with `Drawable`.
  - **CollisionSystem**: Handles collision detection and resolution.
  - **HealthSystem**: Applies damage and checks death.
  - **AISystem**: Updates AI_Brain components.
  - **AttackSystem**: Processes attacks and projectiles.

**Key principle:**  
Systems operate on **data only** and do not store entity-specific state.

---

## Implementing a New Component

To implement a new component:

1. **Define the component data structure**:
   ```cpp
   struct Mana {
       int current;
       int max;
   };
   ```

2. **Register the component** with the Registry:

   ```cpp
   ecs.registerComponent<Mana>();
   ```

3. **Attach the component to entities**:

   ```cpp
   ecs.emplaceComponent(playerEntity, Mana{100, 100});
   ```

4. **Create a system** that operates on entities with this component:

   ```cpp
   ecs.view<Mana>([](Entity e, Mana &mana) {
       // process mana regeneration
   });
   ```

This approach allows **adding new features without modifying existing code**, keeping the ECS modular and scalable.
