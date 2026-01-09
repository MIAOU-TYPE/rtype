# Guide: Adding a New Missile Sprite

This guide explains how to add a new missile sprite to the R-Type project.

## Prerequisites

- PNG file of the sprite (e.g., `missile_new.png`)
- Knowledge of the sprite's dimensions and animation frames

## Steps

### 1. Add the PNG File

Place your PNG file in:
```
client/assets/sprites/missile_new.png
```

### 2. Create the JSON Configuration File

Create `client/assets/json/missile_new.json`:

```json
{
  "sprite": "missile_new",
  "spriteId": 13,
  "texture": "sprites/missile_new.png",
  "default_animation": "fly",
  "animations": {
    "fly": {
      "loop": true,
      "frames": [
        { "x": 0,  "y": 0, "w": 16, "h": 14, "duration": 0.1 },
        { "x": 16, "y": 0, "w": 16, "h": 14, "duration": 0.1 }
      ]
    }
  }
}
```

**Important:** Choose a unique `spriteId` that is not already in use:
- 1-5: enemies and effects
- 6: player missile
- 7-10: players
- 9, 12: enemy missiles
- **13+**: available for new sprites

### 3. Register in AssetLoader

Edit `client/src/utils/assetLoader/AssetLoader.cpp`:

```cpp
Engine::SpriteLoader::loadFromFile("client/assets/json/missile.json", *spriteRegistry);
Engine::SpriteLoader::loadFromFile("client/assets/json/missile_enemy.json", *spriteRegistry);
Engine::SpriteLoader::loadFromFile("client/assets/json/missile_enemy2.json", *spriteRegistry);
Engine::SpriteLoader::loadFromFile("client/assets/json/missile_new.json", *spriteRegistry);  // ← ADD
```

### 4. Add to EmbeddedResourceManager

Edit `client/src/resources/embeddedResource/EmbeddedResourceManager.cpp`:

**A. Declare External Symbols** (at the top of the file):
```cpp
extern const unsigned char missile_new_png[];
extern const unsigned int missile_new_png_size;
```

**B. Register the Resource** (in `registerResources()`):
```cpp
_resources["sprites/missile_new.png"] = {
    EmbeddedResources::missile_new_png, EmbeddedResources::missile_new_png_size};
```

### 5. Regenerate Embedded Resources

```bash
cd build
cmake ..
make
```

CMake will automatically generate `missile_new_png.cpp` from your PNG file.

### 6. Use the New Sprite

You need to use **one or both** methods depending on your use case:

#### A. For Enemies Loaded from Level Files (JSON)

Use this method for enemies that are defined in level configuration files:

```json
{
  "enemies": {
    "superEnemy": {
      "hp": 100,
      "shoot": {
        "projectileSpriteId": 13
      }
    }
  }
}
```

**When to use:**
- For enemies spawned from level data
- When you want designers to modify sprites without touching code
- For static configurations that don't change at runtime

#### B. For Entities Created Programmatically (C++ Code)

Use this method for entities created directly in the game code:

```cpp
// When creating a player or any entity in code
registry.emplaceComponent<Ecs::WeaponConfig>(entity, Ecs::WeaponConfig{13});
```

**When to use:**
- For players (see `World::createPlayer()` in `server/src/game/world/World.cpp`)
- For special bosses or entities created dynamically
- For entities that aren't defined in level files

#### Which Method Do I Need?

- **Enemies in level files**: Use JSON only (option A)
- **Players or dynamic entities**: Use code only (option B)
- **Both types in your game**: Use both methods (they're independent)

**Example:** In R-Type, players are created with `WeaponConfig{6}` in code, while enemies use `projectileSpriteId` from JSON level files. Both coexist in the same game.

## Troubleshooting

### Sprite Does Not Appear
- Verify that the `spriteId` is unique
- Ensure CMake regenerated the resources: check for `missile_new_png.cpp` in `build/client/embedded_resources/`

### Compilation Error "undefined reference"
- You forgot to add the `extern` declarations in EmbeddedResourceManager.cpp
- Or you forgot to reconfigure CMake

### Incorrect Sprite Displays
- ID conflict: ensure your `spriteId` is not already in use
- Check the level JSON files

## Quick Summary

```bash
# 1. Add the files
client/assets/sprites/missile_new.png
client/assets/json/missile_new.json (with unique spriteId)

# 2. Edit the code
AssetLoader.cpp → add loadFromFile()
EmbeddedResourceManager.cpp → add extern + _resources[]

# 3. Recompile
cd build && cmake .. && make

# 4. Use
Level JSON → "projectileSpriteId": 13
```
