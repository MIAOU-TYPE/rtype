# R-Type

A multiplayer shoot 'em up game implemented in C++20, inspired by the classic R-Type arcade game.

## Overview

This project consists of a client-server architecture where players can connect and play together in real-time. The game features:

- Entity Component System (ECS) for game logic
- SFML for graphics and input handling
- UDP-based networking for low-latency communication
- Cross-platform build system using CMake

## Project Structure

The project is organized into the following main directories:

- `client/` - Client application code and assets
- `server/` - Server application code
- `docs/` - Documentation files
- `scripts/` - Build and utility scripts

For detailed information about the directory structure, see [Directory Structure](docss/DIRECTORY_STRUCTURE.md).

## Building

### Prerequisites

- CMake 3.20 or higher
- C++20 compatible compiler
- SFML library (for client graphics)

### Installation

#### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install cmake build-essential clang-format clang-tidy libsfml-dev
```

#### macOS

```bash
brew install cmake sfml llvm  # llvm for clang-format and clang-tidy
```

#### Windows

- Install CMake from [cmake.org](https://cmake.org/download/)
- Install Visual Studio with C++ support
- Install SFML from [sfml-dev.org](https://www.sfml-dev.org/download.php)
- For clang-format and clang-tidy, install LLVM from [llvm.org](https://llvm.org/)

### Build Instructions

```bash
# Clone the repository
git clone <repository-url>
cd rtype

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
make
```

## Running

After building, you can run the server and client executables:

```bash
# Start the server
./r-type_server

# Start the client (in another terminal)
./r-type_client
```

## Documentation

- [Directory Structure](docss/DIRECTORY_STRUCTURE.md) - Detailed project organization
- [Contributing Guidelines](CONTRIBUTING.md) - How to contribute to the project

## Contributing

Please ensure code follows the project's code quality standards. See [Contributing Guidelines](CONTRIBUTING.md) for details on running linting and formatting tools.
