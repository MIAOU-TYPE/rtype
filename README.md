# R-Type

A multiplayer shoot ’em up game implemented in modern **C++20**, inspired by the classic R-Type arcade game.

## Overview

The game is split into two main binaries:

- **Server** (`r-type_server`)
- **Client** (`r-type_client`)

Both are built using **CMake**, and all external dependencies (SFML, GTest, etc.) are managed using **vcpkg**, making the project fully self-contained and easy to build on any platform.

---

## Project Structure

```
client/      # Game client (SFML renderer, input, networking)
server/      # Headless game server (UDP networking, ECS engine)
docs/        # Documentation (technical docs, guides…)
scripts/     # Tools (lint, formatting, CI helpers)
vcpkg.json   # Dependency manifest (SFML, GTest, etc.)
CMakeLists.txt

````

See the detailed description here:  
📄 **[Directory Structure](docs/DIRECTORY_STRUCTURE.md)**

---

## 🔧 Prerequisites

### **All platforms**

- **CMake ≥ 3.20**
- **C++20 compiler**
  - Linux: GCC or Clang  
  - macOS: AppleClang  
  - Windows: MSVC (VS 2022 recommended)
- **Git**
- **vcpkg** (automatically handled by CMake — no manual install required)

Optional:

- `clang-format` if you want to run formatting scripts

---

### ⚠️ **Linux prerequisites (Epitech Docker images)**

On the *Epitech Docker (Epitest)* environment, some base packages are missing.  
You **must** install the following system dependencies:

```bash
apt-get update && apt-get install -y \
    zip unzip tar curl git pkg-config build-essential
````

These are required for:

* bootstrapping `vcpkg`
* building SFML and its sub-dependencies
* compiling CMake projects

This step is only required on minimal environments (like Epitest Docker images).

---

## 🔄 Dependencies through vcpkg

The project uses a **vcpkg manifest (`vcpkg.json`)**, which automatically installs:

* `sfml` (graphics, windowing, audio)
* `gtest` (unit testing)
* other required transitive dependencies

CMake automatically:

1. Downloads vcpkg if missing
2. Bootstraps it
3. Installs all dependencies locally under `build/`

No system-wide installation of SFML or GTest is needed.

---

## 🛠️ Building the Project

### **Linux / macOS**

```bash
git clone <repository-url>
cd rtype

# Configure
cmake -S . -B build

# Build
cmake --build build -j
```

### **Windows (MSVC)**

```powershell
git clone <repository-url>
cd rtype

cmake -S . -B build

cmake --build build
```

---

## ▶️ Running the Game

### **Start the server**

```bash
./r-type_server
```

Windows:

```powershell
./Debug/r-type_server
```

---

### **Start the client**

```bash
./r-type_client
```

Windows:

```powershell
./Debug/r-type_client
```

The client connects to the server using **UDP** for low-latency interactions.

---

## 🧪 Testing

Unit tests use **GoogleTest**, installed automatically through vcpkg:

```bash
cd build
ctest --test-dir build -V
```

---

## 🧹 Code Quality Tools

Lint scripts are available:

```bash
./scripts/lint-check.sh   # Run format checks
./scripts/lint-fix.sh     # Auto-format code
```

The CI pipeline also runs formatting checks.

---

## 📚 Documentation

* 📁 [Directory Structure](docs/DIRECTORY_STRUCTURE.md)
* ✏️ [Contributing Guidelines](docs/CONTRIBUTING.md)
* 🧩 [UX & Accessibility Guidelines](docs/ux_accessibility_guidelines.md)

---

## 🤝 Contributing

Contributions are welcome!
Please follow the coding style enforced by `clang-format` and refer to the contributing guide before submitting a pull request.
