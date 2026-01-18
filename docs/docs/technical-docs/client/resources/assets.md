---
title: Assets
---
#  Assets

This document describes how assets are organized, loaded, and managed on the
client. Assets are treated as **data**, not code, and are consumed through
stable identifiers rather than file paths.

---

## Asset Categories

The client typically manages the following asset types:

- Textures and sprite sheets
- Fonts
- Sound effects and music
- Shaders (optional, backend-dependent)
- Data manifests (JSON, TOML, etc.)

Each category is handled by a dedicated loader or resource manager.

---

## Recommended Layout

A clear, predictable directory structure:


assets/
textures/
fonts/
audio/
levels/


Subdirectories may be added per feature, but the top-level layout should remain
stable.

---

## Asset Identification

- Assets are referenced by logical identifiers, not file paths
- Identifiers remain stable even if files are moved or replaced
- Mapping from identifier → file is centralized

This avoids leaking filesystem details into gameplay or UI code.

---

## Loading Strategy

### Startup
- Preload all assets required for menus and UI
- Load lightweight, frequently used resources early

### Room / Game Entry
- Lazy-load heavy gameplay assets when entering a room
- Avoid loading during active gameplay whenever possible

### Caching
- Loaded assets are cached
- Duplicate loads are forbidden
- Assets remain resident until explicitly released or until shutdown

---

## Asset Embedding

To ensure assets are bundled with the executable and avoid external dependencies,
assets are embedded directly into the client binary at compile time.

### Embedding Process

1. **Asset Discovery**: CMake script (`generate_embedded_assets.cmake`) scans the `client/assets/` directory
2. **Binary Conversion**: Python script (`bin2header.py`) converts binary files to C++ header arrays
3. **Code Generation**: Generated `.cpp` files contain asset data as `unsigned char` arrays
4. **Compilation**: Embedded assets are compiled into the client executable

### Supported Asset Types

The embedding system supports:
- **Images**: PNG sprites and textures
- **Fonts**: OTF and TTF font files
- **Audio**: WAV and FLAC sound files
- **Shaders**: Fragment shaders
- **Data**: JSON configuration files (animations, levels)

### Generated Code Structure

Each asset generates a header file with:
```cpp
namespace EmbeddedResources {
    extern const unsigned char asset_name[];
    extern const unsigned int asset_name_size;
}
```

### Usage in Code

Embedded assets are accessed through the `EmbeddedResourceManager`:
```cpp
auto texture = resourceManager.loadTexture("sprites/player.png");
auto font = resourceManager.loadFont("fonts/main.otf");
auto soundBuffer = resourceManager.loadSound("sounds/shoot.wav");
```

### Build Integration

- Run `generate_embedded_assets.cmake` during the CMake configure step
- Generated files are placed in `build/client/embedded_resources/`
- Assets are automatically linked into the client executable

This approach ensures:
- No external asset files required at runtime
- Faster loading (no filesystem I/O)
- Self-contained executables
- Consistent asset availability across platforms

---

## JSON Asset Definitions

In addition to binary assets, the client uses JSON files to define complex asset behaviors:

### Animation Definitions

JSON files define sprite animations with frame sequences and timing:

```json
{
  "sprite": "player",
  "spriteId": 7,
  "default_animation": "idle",
  "animations": {
    "idle": {
      "loop": true,
      "frames": [
        { "x": 0, "y": 0, "w": 33, "h": 18, "duration": 0.5 },
        { "x": 33, "y": 0, "w": 33, "h": 18, "duration": 0.5 }
      ]
    }
  }
}
```

### Background Definitions

Background layers are defined with scrolling properties:

```json
{
  "spriteId": 101,
  "scrollSpeed": -50.0,
  "tileWidth": 2584.0,
  "tileHeight": 720.0
}
```

### Entity Definitions

Game entities (players, enemies, power-ups) have JSON definitions for sprites, sounds, and behaviors.

---

## Failure Handling

- Missing or invalid assets must not crash the client
- Load failures should:
  - log a warning
  - fall back to a placeholder asset

The game must remain playable even with degraded assets.

---

## Invariants

- Assets are immutable once loaded
- Asset loading never blocks the render loop
- Resource ownership is centralized
- Filesystem structure is invisible to gameplay logic
