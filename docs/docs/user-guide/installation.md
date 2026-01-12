---
title: Installation
---

# Installation

This page explains how to **build and run the R-Type project locally**. It is
intended for both players who want to test the game and developers who need a
local environment for debugging or development.

The project is split into two executables:
- a **server** (authoritative simulation)
- a **client** (presentation, input, and networking)

Both must be built before running multiplayer sessions.

---

## Prerequisites

Before building, ensure the following are available on your system:

### Toolchain
- A C++ compiler supported by the project (GCC, Clang, or MSVC)
- A compatible C++ standard library

### Build system
- **CMake** (minimum version as required by the project)
- A supported dependency manager if used by the project:
  - Conan
  - vcpkg
  - CPM
  - or a project-specific solution

### Networking
- TCP and UDP traffic must be allowed by the OS firewall
- Loopback and/or LAN networking must be functional

### Platform-specific notes
- **Windows**:
  - Visual Studio Build Tools (recommended)
  - or MinGW if explicitly supported by the project
- **Linux / macOS**:
  - standard build-essential / Xcode command-line tools

---

## Building the Project

### Generic CMake build

From the repository root:

```bash
cmake -S . -B build
cmake --build build -j
````

This will:

* configure the project
* build both server and client binaries
* place outputs in the `build/` directory

If the project provides:

* CMake presets
* a wrapper script
* or a custom dependency bootstrap step

use the project-provided command instead of the generic one above.

---

## Running Locally

### Start the Server

Run the server executable first:

```bash
./build/server/rtype_server
```

Expected behavior:

* configuration is loaded and validated
* TCP and UDP sockets are bound
* the server starts listening for clients

If the server accepts arguments (IP, ports, config path, log level), pass them as
documented by the project.

---

### Start the Client

Run the client executable:

```bash
./build/client/rtype_client
```

You may start multiple clients:

* on the same machine (local testing)
* on different machines on the same LAN

Each client connects independently to the server.

---

## Configuration

Common configuration values include:

* **Networking**

    * TCP port (control plane)
    * UDP port (gameplay and snapshots)
    * server bind IP

        * `127.0.0.1` for local-only
        * `0.0.0.0` for LAN access

* **Simulation (server)**

    * tick rate
    * snapshot rate

* **Auth (server)**

    * path to authentication database
    * session and token TTLs

If configuration is file-based:

* ensure the file is present in the working directory
* ensure paths are relative or correctly resolved

Misconfiguration should cause the server to fail fast at startup.

---

## Common Issues and Notes

* If clients cannot connect at all:

    * check firewall rules
    * confirm TCP and UDP ports are open
    * verify server bind IP

* If the client logs in successfully but gameplay never updates:

    * UDP traffic may be blocked
    * the UDP bind step may be failing
    * verify that both client and server agree on UDP port and bind flow

* When testing on a single machine:

    * prefer `127.0.0.1`
    * ensure no other process is using the same ports
