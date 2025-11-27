---
id: rendering
title: Rendering
sidebar_label: Rendering
---

# Rendering System

The rendering system handles drawing all visual elements of the game.

## Features

- Sprite management and animations.
- Background and parallax layers.
- UI rendering (score, health bars).
- Frame rate optimized for performance.

## Workflow

1. Collect all entities with `Sprite` components.
2. Sort layers for correct rendering order.
3. Draw sprites to the screen.
4. Update animations each frame.

## Example

```cpp
for (auto entity : ecs.getEntitiesWith<Sprite>()) {
    renderer.draw(entity.get<Sprite>().texture, entity.get<Position>());
}
