# Architectural Decisions and Technical Justifications

This document presents the main architectural and technological choices made
during the development of the R-Type project.  
Its purpose is to justify these decisions from a performance, scalability,
and maintainability perspective.

This document is descriptive and non-normative.  
Protocol specifications and implementation details are documented separately.

---

## 1. Why UDP for Real-Time Gameplay (Client and Server)

### 1.1 Real-Time Constraints and Latency

A fast-paced game such as R-Type requires minimal end-to-end latency.
Player inputs and entity positions must be transmitted as quickly as possible,
with freshness taking priority over reliability.

UDP allows packets to be sent immediately, without waiting for acknowledgments,
retransmissions, or flow-control mechanisms.  
This results in significantly lower latency compared to TCP.

On a conceptual spectrum:

- TCP prioritizes reliability and ordering, at the cost of latency
- UDP prioritizes speed and immediacy

For a real-time action game, speed is the dominant requirement.

### 1.2 Why TCP Is Counter-Productive for Game State

TCP enforces several mechanisms that are undesirable for real-time simulation:

- guaranteed delivery via retransmission
- strict in-order packet reassembly
- congestion and flow control

These mechanisms can cause packet batching, where multiple delayed packets
arrive at once. In a game context, this may lead to visual freezes,
teleportation effects, or outdated state being applied late.

When entity positions are sent dozens of times per second, retransmitting
old packets is unnecessary and even harmful.

### 1.3 Packet Loss Is Acceptable

In a real-time simulation, losing a single update is not critical.
A newer update will arrive shortly after.

The server continues its simulation independently of individual clients,
and a temporary lag on one client does not block the game loop.

This behavior aligns naturally with UDP’s best-effort delivery model.

### 1.4 Industry Standard Practice

UDP is the standard transport protocol for real-time games,
including fast-paced shooters and competitive titles.

Well-known games such as Overwatch, Valorant, Rocket League, and Fortnite
use UDP-based networking models for real-time state synchronization.

Choosing UDP is therefore a coherent, justified, and widely accepted decision
for a real-time multiplayer game.

---

## 2. Why an Entity Component System (ECS)

The client and server have different responsibilities, but both benefit from
a shared architectural model based on an Entity Component System.

---

## 2.1 Server-Side ECS Benefits

### 2.1.1 Clear Separation of Game Logic

On the server, each entity is represented by a simple identifier.
Behavior is implemented through independent systems such as:

- Movement systems
- Collision systems
- Spawn systems
- Network synchronization systems

This avoids deep inheritance hierarchies and tightly coupled objects.
New gameplay features can be introduced without modifying existing logic.

### 2.1.2 Performance and Scalability

An ECS stores data in contiguous memory structures, enabling efficient iteration.
This is particularly important for a server that must:

- simulate many entities
- at a fixed tick rate
- with predictable performance

This data-oriented approach reduces overhead and improves cache efficiency.

### 2.1.3 Network Serialization

Components are simple, flat data structures.
This makes them well-suited for binary serialization and transmission over UDP.

The ECS architecture therefore integrates naturally with the networking layer.

---

## 2.2 Client-Side ECS Benefits

### 2.2.1 Decoupling Rendering and Logic

On the client, rendering systems only care about visual components
such as transforms and sprites.

Gameplay details such as health, damage, or AI behavior remain server-side.
The client does not require complex gameplay logic to render the scene.

### 2.2.2 Efficient World Synchronization

The server sends authoritative state updates.
The client simply updates or replaces component data accordingly.

This avoids maintaining complex object hierarchies or inheritance-based models
on the client.

### 2.2.3 Simplified Content Extension

Adding a new type of entity does not require refactoring existing code.
It only involves defining a new combination of components.

This makes the architecture flexible and future-proof.

ECS-based designs are widely used in modern engines and frameworks,
including Unity DOTS and other large-scale engines.

---

## 3. Why SFML 3.0.2 for Graphics and Audio

SFML was selected as the multimedia library for the project
because it matches the scope and constraints of an MVP networked game.

### 3.1 Simple and Clear API

The objective of the project is not to build a low-level rendering engine,
but to focus on networking and gameplay.

SFML provides:
- 2D rendering
- sprite and texture management
- keyboard input
- audio playback

All of this is accessible through a minimal and readable API,
which integrates cleanly with an ECS architecture.

### 3.2 Well-Suited for a 2D Shoot’em Up

R-Type is a 2D scrolling game relying on sprites and simple animations.
SFML is well adapted to this type of game and does not introduce unnecessary complexity.

### 3.3 Cross-Platform Support

SFML runs on both Linux and Windows,
which is essential for meeting multi-platform project requirements.

### 3.4 Integrated Audio Support

Audio features such as sound effects and music playback
are provided directly by SFML.

This removes the need for an additional audio library
and simplifies the overall architecture.

### 3.5 Maturity and Community

SFML is a mature library with extensive documentation and community usage,
particularly in educational and prototype contexts.

Version 3 introduces modern C++ support and performance improvements,
making it a reasonable and stable choice for this project.

---

## 4. Why vcpkg as Package Manager

### 4.1 Native Integration with CMake

vcpkg is designed to work directly with CMake.
Dependency integration is straightforward and requires minimal configuration.

This reduces build-system complexity and setup time.

### 4.2 Automated Dependency Management

vcpkg handles downloading, building, and configuring dependencies automatically,
including libraries such as SFML and testing frameworks.

This avoids reliance on system-wide installations.

### 4.3 Multi-Platform Compatibility

vcpkg supports both Linux and Windows toolchains,
including MSVC, which is essential for cross-platform development.

### 4.4 Reproducible Builds

Using a lock file ensures that all team members build against the same
library versions.

This improves stability and reduces integration issues in group projects.

### 4.5 CI/CD Friendly

vcpkg integrates well into continuous integration pipelines,
allowing consistent builds in automated environments.

---

## 5. Summary

The chosen architecture and technologies form a coherent system:

- UDP provides low-latency real-time communication
- TCP is used for reliable control-plane operations
- ECS enables scalable, modular game logic on both client and server
- SFML offers a simple and effective multimedia layer
- vcpkg ensures reproducible and maintainable builds

These decisions are aligned with industry practices
and appropriate for a networked game MVP.
