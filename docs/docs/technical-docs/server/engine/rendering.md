---
id: rendering
title: Rendering
sidebar_label: Rendering
---

# Rendering System

The rendering system handles drawing all visual elements of the game on the server side. Note that the server does not perform actual graphics rendering - it prepares rendering data that is sent to clients via snapshots.

## Features

- Entity visibility management.
- Background and parallax layers.
- Animation state tracking.
- Rendering data serialization for network transmission.

## Components Used

- **Drawable**: Contains sprite ID, layer, and rendering properties
- **Position**: Entity position for rendering
- **Background**: Background layer scrolling and tiling information

## Workflow

1. Collect all entities with `Drawable` components.
2. Update background layer positions based on scroll speed.
3. Prepare rendering data for snapshot transmission.
4. Include animation states and visual effects.

## Server-Side Rendering

Unlike client rendering, server rendering focuses on:
- Determining what entities are visible
- Calculating animation frames
- Preparing background scroll positions
- Ensuring visual state consistency for all clients

The actual drawing is performed by clients using SFML.