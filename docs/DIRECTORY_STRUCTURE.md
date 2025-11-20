# Directory Structure

This document outlines the directory structure of the R-Type project, a multiplayer game implemented in C++ using CMake for build management.

## Overview

The project is organized into modular components to separate client and server logic, assets, documentation, and build scripts.

## Root Directory

```
rtype/
├── CMakeLists.txt          # Root CMake configuration file
├── .clang-format           # Code formatting rules for clang-format
├── .gitignore              # Git ignore patterns
├── client/                 # Client application code and assets
├── server/                 # Server application code
├── docs/                   # Documentation files
└── scripts/                # Build and utility scripts
```

## Client Directory (`client/`)

Contains all client-side code, assets, and configurations.

```
client/
├── assets/                 # Game assets
│   ├── fonts/              # Font files
│   ├── sounds/             # Audio files
│   └── sprites/            # Sprite and texture files
└── src/                    # Source code
    ├── main.cpp            # Client entry point
    ├── ecs/                # Entity Component System
    │   ├── Entity.hpp      # Entity definitions
    │   ├── components/     # ECS components
    │   ├── registry/       # Entity registry
    │   └── systems/        # ECS systems
    ├── events/             # Event handling system
    │   ├── EventBus.hpp    # Event bus implementation
    │   └── EventTypes.hpp  # Event type definitions
    ├── game/               # Game logic
    │   ├── EntitiesFactory.* # Entity creation factory
    │   ├── GameClient.*    # Main game client class
    │   ├── GameScene.*     # Game scene management
    │   └── Starfield.*     # Background starfield effect
    ├── graphics/           # Graphics rendering
    │   ├── interfaces/     # Graphics interfaces
    │   │   ├── IRenderer.hpp
    │   │   ├── ISpriteFactory.hpp
    │   │   └── ITextureManager.hpp
    │   └── SFML/           # SFML implementation
    │       ├── SFMLEntityDrawing.*
    │       ├── SFMLRenderer.*
    │       └── SFMLTextureManager.*
    └── networks/           # Network communication
        ├── interfaces/     # Network interfaces
        │   └── INetworkClient.hpp
        ├── protocol/       # Network protocol definitions
        │   └── ProtocolDefinitions.hpp
        └── udp/            # UDP network implementation
            ├── PacketSerializer.*
            ├── PacketTypes.hpp
            ├── SnapshotParser.*
            └── UdpClient.*
```

## Server Directory (`server/`)

Contains the server-side implementation.

```
server/
├── CMakeLists.txt          # Server-specific CMake configuration
└── src/                    # Server source code
    ├── Main.cpp            # Server entry point
    └── server/             # Server implementation
        ├── interfaces/     # Server interfaces
        │   ├── AServer.*   # Abstract server base class
        │   └── IServer.hpp # Server interface
        └── UDPServer/      # UDP server implementation
            ├── socketParams.hpp
            └── UDPServer.*
```

## Documentation Directory (`docs/`)

Contains project documentation.

```
docs/
└── DIRECTORY_STRUCTURE.md  # This file
```

## Scripts Directory (`scripts/`)

Contains build scripts, deployment scripts, and other utilities.

```
scripts/
└── lint-fix.sh            # Script to auto-format and fix code issues
└── lint-check.sh          # Script to check code formatting and quality
```

## Notes

- All C++ source files use the `.cpp` and `.hpp` extensions
- The project uses C++20 standard
- CMake is used for cross-platform build configuration
- SFML is used for client-side graphics and input handling
- The architecture follows an ECS (Entity Component System) pattern for game logic
- Network communication uses UDP protocol for real-time gameplay
