# Assets and Level

## Goals

A predictable resource layout makes builds reproducible and avoids “it works on my machine”.

In your client, resources are **embedded in the binary** (not read from disk at runtime) and exposed through an `IResourceManager` API. The rest of the client only knows **resource paths** like `sprites/player.png` or `levels/space/easy.json`.

---

## Asset Categories

Your embedded resources currently include:

### Sprites / UI textures

Examples:

* `sprites/player.png`, `sprites/enemy.png`, `sprites/background_space.png`, `sprites/menu_logo.png`, buttons states, etc.

### Shaders

* `shaders/colorblind.frag`

### Fonts

* `fonts/r-type.otf`, `fonts/font.ttf`

### Sounds / Music

* SFX: `sounds/shoot.wav`, `sounds/explosion.wav`, `sounds/hit.wav`, `sounds/powerup.wav`
* Music: `sounds/menu_theme.flac`, `sounds/game_theme.flac`, `sounds/boss_theme.flac`

### Levels (JSON)

* `levels/space/easy.json`, `levels/space/medium.json`, `levels/space/hard.json`

---

## “Folder structure” (virtual paths)

Even if files are embedded, we keep a clean **virtual** layout (the keys you pass to the resource manager):

```text
sprites/
  player.png
  enemy.png
  background_space.png
  ...
fonts/
  r-type.otf
  font.ttf
shaders/
  colorblind.frag
sounds/
  shoot.wav
  explosion.wav
  menu_theme.flac
  ...
levels/
  space/
    easy.json
    medium.json
    hard.json
```

These strings are the “source of truth” used everywhere in the client.

---

## Loading Strategy (as implemented)

### 1) Central entry point: `EmbeddedResourceManager`

At startup, the graphics backend creates an `EmbeddedResourceManager` and exposes it via `SfmlGraphics::resources()`.

The embedded manager registers all resources into an internal map:

* key: `std::string resourcePath`
* value: `{ const unsigned char* data, unsigned int size }`

Loading is then always:

* `loadResource(path) -> (data, size)`
* if not found: `(nullptr, 0)`

### 2) Textures / Fonts / Shaders

**Recommended pattern** (client-side):

* request bytes from `IResourceManager`
* build the SFML object with `loadFromMemory` / similar memory-based APIs

This is consistent with how your audio wrappers work (music/sfx) and prevents filesystem dependency.

### 3) Sounds & Music

Your audio implementation loads bytes from the resource manager and uses SFML memory loaders:

* Music: `sf::Music::openFromMemory(data, size)` (streamed)
* SFX: `sf::SoundBuffer::loadFromMemory(data, size)` (buffered)

So the resource manager is the only “IO layer”; audio stays backend-focused.

### 4) Levels (JSON)

Levels are embedded as JSON blobs and retrieved through the same API:

* `levels/space/easy.json` etc.

Typical flow:

* `loadResource("levels/space/easy.json")`
* parse JSON
* build local “catalog” (for menu thumbnails / difficulty selection) or send selection to server

Even if the server owns authoritative gameplay, the client still needs some level-related content:

* menu labels / difficulty catalog
* background theme selection
* preview thumbnails (if you add them later)

---

## Simple Diagram: Resource Flow (embedded → runtime objects)

```text
┌──────────────────────────────────────────────────────────────┐
│                      EmbeddedResourceManager                 │
│   map[path] = { data ptr, size }                             │
│   loadResource(path) → (data,size) or (nullptr,0)            │
└───────────────▲──────────────────────────────────────────────┘
                │ resourcePath (string)
                │
┌───────────────┴───────────────┐
│     Subsystems (client)       │
│  - Renderer (textures/shaders)│
│  - UI (fonts, sprites)        │
│  - Audio (music/sfx)          │
│  - Levels (json parser)       │
└───────▲───────────▲───────────┘
        │           │
        │ bytes     │ bytes
        │           │
┌───────┴───────┐  ┌───────────────────────────────┐
│ SFML Objects  │  │   Parsed Content / Catalog    │
│ - Texture     │  │ - Level definitions           │
│ - Font        │  │ - Menu previews (optional)    │
│ - Shader      │  └───────────────────────────────┘
│ - Music/Sound │
└───────────────┘
```

## Practical Guidelines

* Prefer **stable, lowercase paths** (your current style is consistent)
* Keep **format conventions**:

    * `.png` for sprites/UI
    * `.frag` for shaders
    * `.wav` for short SFX
    * `.flac` for music (or `.ogg`)
    * `.json` for levels
* Don’t let gameplay code access raw files: always go through `IResourceManager`
