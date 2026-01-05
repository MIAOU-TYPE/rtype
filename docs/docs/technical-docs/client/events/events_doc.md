# Inputs & Events

## Goals

The input/event layer provides a clean separation between:

* **Platform / window events** (SFML, OS, etc.)
* **Game/UI logic** (menus, gameplay states)
* **Network actions** (sending input packets)

This client uses an **event-driven architecture** built around:

* `EventBus`: type-based event dispatching
* `EventRegistry`: convenience layer for binding callbacks to specific keys
* `InputState`: per-frame aggregated input snapshot for UI/state updates

---

## Architecture Overview

```text
┌──────────────────────────┐
│     Window Backend       │  (SFML / platform)
│  - polls OS events       │
└─────────────┬────────────┘
              │ emits typed events
              ▼
┌──────────────────────────┐
│         EventBus         │
│  - on<T>(handler)        │
│  - emit(T event)         │
│  - dispatch()            │
└───────┬───────────┬──────┘
        │           │
        │           │ direct listeners
        │           ▼
        │   ┌──────────────────────┐
        │   │     InputState       │
        │   │ - setKey/mouse...    │
        │   │ - consumeFrame()     │
        │   └──────────┬───────────┘
        │              │ InputFrame
        ▼              ▼
┌──────────────────┐  ┌──────────────────────────┐
│   EventRegistry  │  │     StateManager/UI      │
│ - binds callbacks│  │ update(frame inputs)     │
│   to KeyPressed/ │  │ render()                 │
│   KeyReleased    │  └──────────────────────────┘
└──────────────────┘
```

---

## EventBus

`EventBus` is a small typed event system:

* `on<T>(handler)` registers a handler for type `T`
* `emit(T event)` queues an event (thread-safe)
* `dispatch()` drains the queue and calls handlers

### Dispatch model

* `emit()` pushes a `QueuedEvent` into `_queue` guarded by `_queueMutex`.
* `dispatch()` swaps the queue into a local queue, then invokes callbacks without holding the mutex.

This ensures:

* event emission is safe from any producer context
* dispatch is deterministic and avoids handler execution under lock

### Notes / constraints

* Events are matched strictly by **typeid(T)**.
* Ordering is FIFO within the queue.
* Handlers are stored as `std::function<void(const void*)>` (type-erased).

---

## Events (Engine::Event)

The client defines a minimal set of typed events:

* `WindowClosed`
* `KeyPressed { Key key }`
* `KeyReleased { Key key }`
* `MouseMoved { int posX, posY }`
* `MousePressed { Key key, int posX, posY }`
* `MouseReleased { Key key, int posX, posY }`
* `WindowResized { uint32_t width, height }`

`Key` is an internal enum representing keyboard and mouse inputs (`Escape`, arrows, `Space`, `MouseLeft`, etc.).

---

## EventRegistry (key binding helper)

`EventRegistry` builds on top of the `EventBus`:

* Subscribes to `KeyPressed` and `KeyReleased` once
* Internally maps:

    * `Key -> vector<function<void()>>` for pressed callbacks
    * `Key -> vector<function<void()>>` for released callbacks

### Usage

```cpp
Engine::EventRegistry reg(eventBus);

reg.onKeyPressed(Engine::Key::Escape, [&] { runtime.stop(); });
reg.onKeyReleased(Engine::Key::Space,  [&] { /* shoot */ });
```

Why it exists:

* Most gameplay/UI code does not need the full event payload.
* Key bindings become easy to read and keep centralized.

---

## InputState (per-frame aggregation)

`InputState` is a light “input snapshot” system intended for **UI/state updates**:

* During event dispatch, listeners update `InputState` via:

    * `setMouse(x, y)`
    * `setMousePressed() / setMouseReleased()`
    * `setKeyPressed(key) / setKeyReleased(key)`
* Each display tick, UI logic consumes a single `InputFrame`:

```cpp
InputFrame frame = input.consumeFrame();
stateManager.update(frame);
```

### consumeFrame() semantics

`consumeFrame()` returns the current frame data and clears **edge flags**:

* `mousePressed`, `mouseReleased`
* `keyPressed`, `keyReleased`
* resets `key` to `Unknown`

This means:

* it is designed for “event-like” UI interactions (clicks, single press)
* it is not a full “held keys” state tracker by itself

---

## How it ties into the Game Loop

In the display loop:

1. backend polls window events and emits typed `Event` objects into the `EventBus`
2. `EventBus::dispatch()` runs all handlers
3. some handlers update:

    * `InputState` (for UI/state consumption)
    * or call `sendPacket(...)` directly for immediate network actions
4. `StateManager.update(input.consumeFrame())` uses the per-frame snapshot to drive menus/UI

This gives two complementary input paths:

* **Event-driven actions**: immediate reactions (quit, shoot on release, etc.)
* **Frame-driven UI**: menus and buttons based on `InputFrame`

---

## Recommended conventions (project-friendly)

### 1) Keep backend-specific code out of gameplay

Only the window backend should translate SFML events into `Engine::Event` types.

### 2) Prefer “edge events” for UI

Buttons should react to `mousePressed/mouseReleased` rather than “held state”.

### 3) Avoid sending network packets every frame by default

Use:

* `KeyPressed` / `KeyReleased` bindings for discrete actions
* or a fixed-rate input sender (optional) if you need continuous movement updates

---

## Limitations and possible improvements

### A) InputFrame stores a single key

Right now `InputState` keeps only one `Key` field per frame.
If multiple key events happen in the same tick, later ones overwrite earlier ones.

Improvement options:

* store a small vector of key events in the frame
* store a bitset of held keys + edge flags

### B) WindowResized does not inherit from Event

`WindowResized` is not derived from `Event` (unlike other events).
That’s fine if you still dispatch by type, but for consistency you may want it to inherit too.

### C) Separate “UI inputs” vs “Gameplay inputs”

If your gameplay needs continuous “held keys”, add:

* `InputState::setHeld(Key, bool)` and expose `isHeld(Key)` in `InputFrame`
  or
* an `InputManager` that maintains a pressed bitset.
