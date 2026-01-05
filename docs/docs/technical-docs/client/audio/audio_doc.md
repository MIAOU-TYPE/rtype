---
id: audio
title: Audio System
sidebar_label: Audio System
---

# Audio System

## Scope

The client audio layer is responsible for:

* loading audio assets (music and SFX) using the client `ResourceManager` (memory-based loading)
* playing and stopping audio via opaque handles (`AudioHandle`)
* per-audio volume & looping controls
* a *global* volume multiplier affecting currently active audio

This module is split into **two backends**:

* **Music** (streamed) via `sf::Music` opened from memory
* **SFX** (buffered) via `sf::SoundBuffer` + `sf::Sound`

---

## Current Architecture (as implemented)

```
Graphics/
├── IAudioManager.hpp                # Engine-agnostic interface
├── SfmlMusic.hpp/.cpp               # Wrapper around sf::Music (openFromMemory)
├── SfmlSound.hpp/.cpp               # Wrapper around sf::SoundBuffer + sf::Sound (loadFromMemory)
├── SfmlMusicManager.hpp/.cpp        # Manages music handles -> SfmlMusic instances
└── SfmlSoundManager.hpp/.cpp        # Manages SFX handles + active playing instances
```

### Key types

* `using AudioHandle = std::size_t;`
* `constexpr AudioHandle InvalidAudio = 0;`
* Valid handles start at `1` (`_nextHandle` resets to `1` after `clear()`)

### Error handling

* Music / Sound wrappers throw `AudioError` when the resource cannot be loaded/opened.

---

## Resource Loading Model

### Music (`SfmlMusic`)

* Loads the file bytes through `Resources::IResourceManager::loadResource(path)`
* Uses `sf::Music::openFromMemory(data, size)`
* Good for longer tracks (streamed)

### SFX (`SfmlSound`)

* Loads the file bytes through `Resources::IResourceManager::loadResource(path)`
* Uses `sf::SoundBuffer::loadFromMemory(data, size)`
* Intended for short sounds (buffered)

---

## Managers Behavior

### `SfmlMusicManager`

**What `load(path)` does**

* Deduplicates by path: if already loaded, returns the existing handle
* Otherwise creates a `SfmlMusic` and stores it in `_musics[handle]`

**play/stop/volume/loop**

* `play(handle)` starts the `sf::Music`
* `stop(handle)` stops it
* `setVolume(handle, volume)` is **0..100** (SFML scale)
* `setLooping(handle, bool)` toggles looping

**clear()**

* stops any playing musics
* clears maps and resets handle counter

**global volume**

* `setGlobalVolume(multiplier)` currently multiplies the *current* volume of **playing** musics:

    * only affects tracks that are **already playing**
    * repeated calls compound (e.g. calling with `0.5` twice will quarter the volume)

---

### `SfmlSoundManager`

**What `load(path)` does**

* Checks the resource exists (loads bytes once for validation)
* Stores only the `resourcePath` per handle (no buffer cached at load-time)

**What `play(handle)` does**

* Creates a *new* `SfmlSound` instance (buffer + sound) each time you play
* Calls `play()` and pushes it into `_activeSounds`
* This allows overlapping playback of the same sound

**stop(handle)**

* Stops and removes every active sound instance that matches the handle

**setVolume / setLooping**

* Updates only the **currently active** instances of that handle

**clear()**

* clears loaded handles and currently active sounds
* resets handle counter

**global volume**

* `setGlobalVolume(multiplier)` multiplies the current volume of **all active sounds**
* repeated calls compound the same way as music

---

## Volume Strategy (recommended UI mapping)

Your backend API uses **SFML volume scale**:

* `0.0 .. 100.0`

Recommended UI storage:

* store in config/UI as `0..1` floats
* convert to SFML with: `sfmlVolume = v01 * 100.0f`

### “Music vs SFX” separation

Since you have **two managers**, the clean approach is:

* `musicVolume01` → applied to currently playing music handle via `SfmlMusicManager::setVolume(handle, ...)`
* `sfxVolume01` → applied to new SFX instances (and optionally updated on active ones)

> Note: right now the SFX volume is set per active instance; if you change the slider, you should also iterate current `_activeSounds` (which you already do via `setVolume(handle, ...)`).

---

## Triggering Sounds

Trigger audio from **events**, not from raw entity state:

* UI click → play “Click” SFX handle
* input “shoot pressed” → play “Shoot” handle once
* network event “enemy destroyed” → play “Explosion” handle once
* scene change (menu → game) → stop menu music, start game music

This avoids replaying the same sound every frame.

---

## Assets Layout

```
client/assets/
├── sounds/
│   ├── shoot.wav
│   └── explosion.wav
...
```

Formats:

* `.wav` for short SFX
* `.flac` for music