# R-Type — Networked Shoot’em Up (C++20)

R-Type is a **networked multiplayer shoot’em up** inspired by the classic R-Type, developed in **C++20** as part of an EPITECH Advanced C++ project.

The project is composed of:
- **`r-type_server`** — a **multi-threaded authoritative UDP server** (game logic and rules)
- **`r-type_client`** — a **graphical SFML client** (rendering, input, UI)

The server is the single source of truth: all gameplay decisions are validated server-side.

---

## Quick start (Linux)

```bash
git clone <repository_url>
cd r-type
cmake -S . -B build
cmake --build build -j
./r-type_server --port 8080
./r-type_client
````

> Binaries are generated inside the `build/` directory.

---

## Features

* Authoritative server architecture (server owns the game state)
* Custom **binary UDP protocol** (versioned headers + typed packets)
* Multi-threaded server runtime:

    * Network I/O
    * Game logic processing
    * Fixed tick/update loop
    * Snapshot broadcasting
* ECS-based server-side game logic
* Modular shared libraries (protocol, packets, buffers, networking)
* SFML client with rendering, input handling, menu scene and scrolling starfield
* Cross-platform build (Linux / Windows)

---

## Architecture overview

### Client / Server model

* Clients send **inputs only**
* The server processes inputs, updates the game world and sends snapshots
* Clients render the received snapshots

This ensures consistency and prevents client-side authority.

### Server threading model

The server runtime is split into dedicated threads:

* UDP receive/send thread
* Game logic thread (ECS systems)
* Fixed-rate tick/update thread
* Snapshot generation and broadcast thread

This prevents network I/O from blocking gameplay updates.

---

## Requirements

### Build tools

* **CMake ≥ 3.20**
* **C++20 compiler** (GCC / Clang / MSVC)
* **Git**
* **Python 3** (used at configure time to embed client assets)

### Linux system packages (recommended)

On Debian / Ubuntu-based systems:

```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential cmake git ninja-build python3 \
  zip unzip curl pkg-config \
  libx11-dev libxrandr-dev libxcursor-dev \
  libxi-dev libudev-dev libgl1-mesa-dev
```

> All third-party dependencies are handled through **vcpkg**, automatically fetched by CMake.

---

## Build

From the repository root:

```bash
cmake -S . -B build
cmake --build build -j
```

### Build only server or client

```bash
cmake -S . -B build -DBUILD_SERVER_ONLY=ON
# or
cmake -S . -B build -DBUILD_CLIENT_ONLY=ON
```

---

## Run

### Start the server

Linux:

```bash
./r-type_server --port 8080
```

Windows:

```powershell
cd bin
./r-type_server.exe --port 8080
```

To display all available options:

```bash
./r-type_server --help
```

### Start one or more clients

Linux:

```bash
./r-type_client
```

Windows:

```powershell
cd bin
r-type_client.exe
```

> Make sure the **client targets the same IP and port** as the server.

---

## Controls (client)

Default mappings:

* Move: **Arrow keys** or **Z Q S D**
* Shoot: **Space** or **Left Ctrl**
* Pause: **P**
* Quit: **Escape**

---

## Network protocol

The game uses a **custom binary UDP protocol**:

* Versioned packet headers
* Typed payloads (inputs, entity state, events)
* Fixed-rate server snapshots

The protocol is designed to be compact and resilient to malformed packets.

> See `docs/protocol.md` for the full specification.

---

## Tests

Enable tests at configure time:

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build -j
ctest --test-dir build -C Debug -V
```

---

## Project layout

```
server/   — authoritative server, ECS, runtime threads, networking
client/   — SFML client (rendering, input, scenes, embedded assets)
shared/   — reusable libraries (protocol, packets, buffers, net wrapper)
docs/     — technical documentation
```

---

## Troubleshooting

### Client cannot connect

* Ensure server and client use the same IP and port
* Check that no firewall blocks UDP traffic

### Dependency issues

* Dependencies are managed by vcpkg
* Delete `build/` and reconfigure if necessary

---

## Project members
- [Anna POGHOSYAN](https://github.com/ann7415)
- [Evann BLOUTIN](https://github.com/EvannBloutin)
- [Jules FAYET](https://github.com/julesfayet)
- [Santiago PIDCOVA](https://github.com/santiagopidji)
- [Robin SCHUFFENECKER](https://github.com/rosh7887epitech)
- [Romain BERARD](https://github.com/romain1717)
---

## License

Educational project — no license file provided.