# R-Type

A multiplayer shoot ’em up game implemented in modern **C++20**, inspired by the classic R-Type arcade game.

## Overview

The game is split into two main binaries:

* **Server** (`r-type_server`)
* **Client** (`r-type_client`)

Both are built using **CMake**, and all external dependencies (SFML, GTest, etc.) are managed using **vcpkg**, making the project fully self-contained and easy to build on any platform.

---

## Project Structure

```
client/      # Game client (SFML renderer, input, networking)
server/      # Headless game server (UDP networking, ECS engine)
docs/        # Documentation (directory structure, contributing guide…)
scripts/     # Tools (lint, formatting, CI helpers)
vcpkg.json   # Dependency manifest (SFML, GTest, etc.)
CMakeLists.txt
```

See the detailed description here:
📄 **[Directory Structure](docs/DIRECTORY_STRUCTURE.md)**

---

## Building the Project

### 🔧 Prerequisites (all platforms)

* **CMake ≥ 3.20**
* **C++20** compiler

    * Linux: GCC or Clang
    * Windows: MSVC (Visual Studio 2022 recommended)
    * macOS: AppleClang
* **Git**
* **vcpkg (automatically fetched by CMake)**
  → No manual installation required
* (Optional) `clang-format` if you want to run lint locally

---

## 🔄 Dependencies through vcpkg

The project uses a `vcpkg.json` manifest listing required packages:

* `sfml` (client)
* `gtest` (unit tests)
* Other audio/graphics dependencies handled automatically

CMake automatically:

1. Downloads vcpkg (if missing)
2. Bootstraps it
3. Installs dependencies locally inside the `build/` folder

No system-wide SFML installation is needed.

---

## 🛠️ Build Instructions (Cross-Platform)

### Linux / macOS

```bash
git clone <repository-url>
cd rtype

# Configure
cmake -S . -B build

# Build
cmake --build build -j
```

### Windows (MSVC)

```powershell
git clone <repository-url>
cd rtype

cmake -S . -B build

cmake --build build
```

---

## ▶️ Running the Game

### Start the server:

```bash
./r-type_server
```

#### Start the server on Windows (MSVC):

```bash
./Debug/r-type_server
```

### Start the client (in another terminal):

```bash
./r-type_client
```

#### Start the client on Windows (MSVC):

```bash
./Debug/r-type_client
```

The client connects to the server via UDP to allow low-latency multiplayer gameplay.

---

## 🧪 Testing

Unit tests use **GoogleTest** (installed automatically via vcpkg):

```bash
cd build
ctest --test-dir build -V
```

---

## 🧹 Code Quality

This project includes formatting and linting scripts:

```bash
./scripts/lint-check.sh   # Format check
./scripts/lint-fix.sh     # Auto-fix formatting
```

The CI also runs the linter automatically.

---

## 📚 Documentation

* 📄 **[Directory Structure](docs/DIRECTORY_STRUCTURE.md)**
* 📄 **[Contributing Guidelines](docs/CONTRIBUTING.md)**

## 🤝 Contributing

Contributions are welcome!
Please follow the coding style enforced by clang-format and refer to the contributing guide before opening a PR.
