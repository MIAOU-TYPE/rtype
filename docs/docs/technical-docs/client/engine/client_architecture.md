# Client Architecture

## Overview

The client is responsible for **displaying the game**, **collecting player input**, and **communicating with the server**.
Gameplay rules and authoritative state live on the server: the client should never be the final judge for critical events (kills, pickups, score, etc.).

## High-level Modules

A typical client build is split into a few decoupled subsystems:

```
Client
├── Graphics / Rendering        # Window, sprites, UI rendering
├── Input                       # Keyboard/mouse/controller -> actions
├── Networking                  # UDP/TCP transport, packet encoding/decoding
├── Game State                  # Local representation of entities received from server
├── Scenes / Menus              # Main menu, lobby/rooms, in-game, settings
├── Audio (optional)            # Music, SFX, mixer
└── Resources                   # Assets loading + caching
```

### Why this split?

* It makes the client easier to test and evolve.
* Each module has a single job (render, input, network…), and communicates through clear interfaces.
* You can swap implementations (SFML vs another renderer) without rewriting the whole client.

## Data Flow

### 1) Input → Network

1. Poll OS/window events (SFML events).
2. Convert raw inputs to **actions** (move, shoot, menu navigation).
3. Encode actions into binary packets.
4. Send to server via UDP.

### 2) Network → Game State → Render

1. Receive server snapshots/events (UDP).
2. Decode packets and update the local game state (entities/components).
3. Render the current state (interpolated if needed).

## Threading Model

Even if your current prototype runs everything in one loop, it is helpful to think in terms of responsibilities:

* **Main thread**: window events + scene update + rendering.
* **Network thread** (optional): blocking receive, decoding, pushing updates to a thread-safe queue.

If you go multi-threaded on the client, keep cross-thread communication simple:

* One producer/consumer queue for incoming messages.
* No SFML rendering calls outside the main thread.

## Interfaces to keep

You already introduced interface-based abstractions for rendering and input. Keep the same idea for:

* `INetworkClient` (send/receive, connect/disconnect)
* `IClock` / `ITimer` (for deterministic updates)
* `IAudio` (if you add audio later)

This gives you a stable API surface while the internals evolve.

## Related documentation

* Networking packet creation (PacketFactory): `../network/client_network_doc.md`
* SFML renderer + input wrapper: `../graphic/graphic_doc.md`
