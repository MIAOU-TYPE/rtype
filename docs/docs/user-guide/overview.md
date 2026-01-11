---
title: Overview
---

# Overview

This user guide documents how to **install, run, and use** the R-Type project from
a user and developer perspective. It focuses on *how the system is used*, not on
implementation details (which are covered in the technical documentation).

The guide is intended for:
- **Players** who want to launch the client and join a multiplayer session
- **Developers** who want to run the server locally for testing or debugging
- **Contributors** who need a clear mental model of runtime flow and responsibilities

---

## What You Get

The project is composed of two independent executables:

### Client
- Menus and navigation (login, lobby, rooms)
- Gameplay rendering (graphics, animations, UI)
- Audio playback (music and sound effects)
- Input capture and networking
- Snapshot interpolation and presentation

### Server
- Authoritative game simulation
- Room and session management
- Authentication and access control
- Input validation and state synchronization
- Snapshot generation and replication

The client is **responsive and presentation-focused**.  
The server is **authoritative and deterministic**.

---

## Typical Multiplayer Flow

A standard session follows this sequence:

1. **Start the server**
    - Opens TCP and UDP listeners
    - Initializes configuration, persistence, and simulation

2. **Launch one or more clients**
    - Each client connects independently

3. **Authenticate**
    - Login or register via TCP
    - Session is established server-side

4. **Enter the lobby**
    - List existing rooms
    - Create a new room or join an existing one

5. **Play**
    - Server starts the game
    - Clients send inputs via UDP
    - Server sends authoritative snapshots

6. **End of game**
    - Return to lobby or main menu
    - Session remains valid until logout or disconnect

---

## Quick Start (Local Development)

For a local setup:

1. Start the server executable
2. Launch one or more client instances
3. Login or register from the client UI
4. Create or join a room in the lobby
5. Start the game when ready

This flow works on a single machine or across multiple machines on the same LAN,
provided TCP and UDP ports are accessible.

---

## How to Use This Guide

- **Installation**  
  Build requirements, dependencies, and how to run the binaries.

- **Usage**  
  Day-to-day runtime behavior for server and client.

- **Mechanics and Roles**  
  Clear separation of responsibilities between client and server.

- **Tips**  
  Troubleshooting, common mistakes, and debugging advice.

For architectural and protocol-level details, refer to the **Technical
Documentation** section.
