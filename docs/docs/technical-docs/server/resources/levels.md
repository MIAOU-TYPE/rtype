---
id: levels
title: Levels and Gameplay
sidebar_label: Levels and Gameplay
---

# Levels and Gameplay

This document describes the level system, enemy definitions, power-ups, and boss mechanics implemented in the R-TYPE server.

## Level Structure

Levels are defined in JSON files located in the `levels/` directory. Each level contains:

### Level Metadata
- `name`: Display name of the level
- `duration`: Total level duration in seconds
- `background`: Background scrolling configuration

### Enemy Definitions
Enemies are defined in the `enemies` object with the following properties:

```json
{
  "smallEnemy": {
    "hp": 15,
    "speed": -100,
    "size": { "w": 65, "h": 66 },
    "killScore": 30,
    "sprite": "enemy",
    "spriteId": 2,
    "shoot": {
      "type": "straight",
      "cooldown": 2.0,
      "projectileSpeed": 130,
      "damage": 50,
      "muzzle": { "x": -20, "y": 50 },
      "projectileSpriteId": 9
    }
  }
}
```

**Enemy Properties:**
- `hp`: Hit points
- `speed`: Movement speed (negative = leftward)
- `size`: Collision box dimensions
- `killScore`: Points awarded for destruction
- `sprite`: Asset identifier
- `spriteId`: Numeric sprite ID
- `shoot`: Shooting behavior configuration

**Shooting Types:**
- `straight`: Single projectile forward
- `diagonal`: Projectiles at specified angles
- `spread`: Multiple projectiles in a fan pattern

### Enemy Groups
Enemies can be grouped for formation spawning:

```json
{
  "groupEnemy": {
    "type": "group",
    "members": [
      {
        "enemyType": "mediumEnemy",
        "offset": { "x": 0, "y": 0 }
      },
      {
        "enemyType": "smallEnemy",
        "offset": { "x": 0, "y": -130 }
      }
    ]
  }
}
```

### Obstacles
Levels can include environmental hazards:

```json
{
  "gravityWell": {
    "spriteId": 15,
    "size": { "w": 34, "h": 34 },
    "pullStrength": 250,
    "damagePerSecond": 10,
    "radius": 140,
    "innerRadius": 50
  }
}
```

### Waves
Levels are structured as a sequence of waves:

```json
{
  "waves": [
    {
      "time": 1,
      "enemies": { "smallEnemy": 1 },
      "spawnPattern": "line",
      "spawnY": 360
    }
  ]
}
```

**Wave Properties:**
- `time`: Spawn time in seconds
- `enemies`: Enemy counts by type
- `spawnPattern`: "line" (centered), "spread" (distributed), or empty (random)
- `obstacleType`: Optional obstacle to spawn
- `powerUps`: Number of power-ups to spawn

## Power-Up System

Power-ups enhance player capabilities and are collected by flying through them.

### Power-Up Types

1. **Force Power-Up** (`force`): Increases projectile damage and size
2. **Laser Power-Up** (`laser`): Charges a powerful laser beam weapon
3. **Bubble Power-Up** (`bubble`): Creates a protective shield

### Power-Up Mechanics

- **Collection**: Player must collide with floating power-up entities
- **Attachment**: Power-ups attach to the player's ship visually
- **Activation**: Special weapons activated with dedicated input (E key)
- **Duration**: Power-ups have limited uses or time limits
- **UI**: Power-up bars show charge levels and cooldowns

### Bubble Power-Up
- Creates expanding bubble shield around player
- Provides collision immunity for a short duration
- Automatically activates when charged

### Laser Power-Up
- Charges over time when attached
- Fires a powerful piercing laser beam
- High damage but long cooldown

## Boss System

Bosses are complex enemies with multiple phases and behaviors.

### Boss Components

- **BossPart**: Defines multiple hitboxes with different damage multipliers
- **BossPhase**: Manages phase transitions based on health thresholds
- **TailFollower**: Handles segmented boss tails that follow the main body

### Boss Mechanics

- **Multi-Part**: Bosses can have weak points and armored sections
- **Phase Transitions**: Behavior changes as health decreases
- **Complex Movement**: Advanced AI patterns and attack telegraphs
- **Tail Segments**: Connected body parts with following physics

### Boss Phases

Bosses transition between phases at health thresholds:

```cpp
struct BossPhaseData {
    float healthThreshold;    // Health percentage to trigger phase
    std::string behavior;     // Phase-specific behavior
    float speedMultiplier;    // Movement speed change
    // Additional phase properties...
};
```

## Level Loading

Levels are loaded at runtime using the `Level` class:

1. **Parsing**: JSON files are parsed into `Level` structures
2. **Validation**: Enemy types and wave data are validated
3. **Difficulty Scaling**: Enemy properties are modified based on difficulty settings
4. **Runtime Spawning**: Waves spawn enemies at specified times during gameplay

### Difficulty Modifiers

```cpp
struct DifficultyModifiers {
    float enemyHpMultiplier = 1.0f;
    float enemyDamageMultiplier = 1.0f;
    float enemySpawnRateMultiplier = 1.0f;
    float projectileSpeedMultiplier = 1.0f;
    float enemySpeedMultiplier = 1.0f;
    float enemyScoreMultiplier = 1.0f;
};
```

These modifiers scale enemy difficulty while maintaining balanced gameplay.