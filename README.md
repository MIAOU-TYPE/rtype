# R-Type (Networked Shoot'em Up)

R-Type is a C++20 multiplayer shoot’em up inspired by the classic R-Type, built as an EPITECH Advanced C++ project.

It ships:
- **`r-type_server`**: a **multi-threaded authoritative** UDP server (game rules run on the server).
- **`r-type_client`**: a **graphical SFML** client (rendering + input).

## Features

- Authoritative server architecture (server decides the “truth”).
- UDP binary protocol (versioned header + typed packets).
- Server runtime split into dedicated threads (I/O, processing, tick/update, snapshots).
- ECS-based game logic (server-side) and modular shared libraries.
- Client with SFML rendering, menu scene, and a scrolling starfield.

## Requirements

### Build tools
- **CMake ≥ 3.20**
- A **C++20** compiler (GCC / Clang / MSVC)
- **Git**
- **Python 3** (used at configure time to embed client assets)

### Linux system packages (recommended)
On Debian/Ubuntu-like systems:
```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential cmake git ninja-build python3 \
  zip unzip curl \
  libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev libgl1-mesa-dev
````

> Dependencies are handled through **vcpkg** (auto-fetched by CMake).

## Build

From the repository root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Binaries are generated in the repository root:

* `./r-type_server`
* `./r-type_client`

> Important: the networking plugin library is loaded from `build/` by default (dynamic loading expects `build/libNetPluginLib.so` on Linux).

### Build only client or server

```bash
cmake -S . -B build -DBUILD_SERVER_ONLY=ON
# or
cmake -S . -B build -DBUILD_CLIENT_ONLY=ON
```

## Run

### 1) Start the server

```bash
./r-type_server
```

### 2) Start one or more clients (in other terminals)

```bash
./r-type_client
```

## Controls (client)

Default mappings:

* Move: **Arrow keys** or **Z Q S D**
* Shoot: **Space** or **Left Ctrl**
* Quit: **Escape**
* Pause: **P**

## Current network configuration

The server port is configurable through command-line arguments.

- **Server**: binds to `127.0.0.1:<port>` where `<port>` comes from the argument parser  

Example:
```bash
./r-type_server --port 8080
````

To display all available options:

```bash
./r-type_server --help
```

> Make sure the **client targets the same IP/port** as the server.

## Tests

Configure with tests enabled:

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build -j
ctest --test-dir build
```

## Project layout

* `server/` — authoritative server, networking, runtime threads, ECS + systems
* `client/` — SFML client (rendering, input, scenes, embedded assets)
* `shared/` — reusable libraries (packets, protocol, buffers, dynamic loading, net wrapper)

## Linting

Utility scripts are available:

* `scripts/lint-check.sh`
* `scripts/lint-fix.sh`

## Docker (Linux testing)

A `Dockerfile` is provided for quick Linux builds:

```bash
docker build -t rtype .
docker run -it --rm rtype
```

## License

Educational project (no license file provided).

## Project members
- [Anna POGHOSYAN](https://github.com/ann7415)
- [Evann BLOUTIN](https://github.com/EvannBloutin)
- [Jules FAYET](https://github.com/julesfayet)
- [Santiago PIDCOVA](https://github.com/santiagopidji)
- [Robin SCHUFFENECKER](https://github.com/rosh7887epitech)
- [Romain BERARD](https://github.com/romain1717)
