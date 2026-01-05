# Levels

## Goals

Level files are **content**, not code. They should be easy to:

* list in menus (difficulty/world selection)
* load reliably (stable schema + validation)
* extend over time (add enemies, waves, patterns) without breaking older clients

Your project uses **two layers**:

1. A **catalog** JSON (list of available levels for a world/difficulty)
2. A **level definition** JSON (the actual gameplay content: enemies + waves)

---

## 1) Level Catalog Files

Catalogs are lightweight lists used by the client to populate menus.

Example: `assets/levels/space/easy.json`

```json
[
  {
    "id": "space_easy_01",
    "name": "Space - Tutorial Drift",
    "path": "client/assets/levels/space/space_easy_01.json"
  }
]
```

### Fields

* `id` (string, unique): stable identifier used internally (and potentially by the server)
* `name` (string): user-friendly name shown in UI
* `path` (string): path to the **level definition** JSON

### Rules

* `id` must be **unique across all catalogs** if you ever merge them (recommended).
* `path` should be a stable virtual path (not an absolute filesystem path).
* Keep catalogs **small** and **purely descriptive**: no gameplay values here.

### Recommended organization

```text
assets/levels/
  space/
    easy.json
    medium.json
    hard.json
  jungle/
    easy.json
    ...
```

Each catalog file returns “available level definitions”.

---

## 2) Level Definition Files

A level definition is the actual content: duration, enemy templates, and waves.

Example: `levels/space_easy_01.json`

```json
{
  "name": "Space - Tutorial Drift",
  "duration": 60.0,
  "enemies": { ... },
  "waves": [ ... ]
}
```

### Core fields

#### `name`

* string displayed in UI / logs

#### `duration`

* total duration in seconds
* used to stop spawning and end the level cleanly

#### `enemies`

A dictionary of **enemy templates** keyed by a logical id (e.g. `"drone"`, `"shooter"`).

Each enemy template contains base stats and optional shooting configuration.

Example template (simplified):

```json
"drone": {
  "hp": 30,
  "speed": -90.0,
  "size": { "w": 28.0, "h": 22.0 },
  "spriteId": 10,
  "killScore": 10,
  "shoot": { "type": "straight", "cooldown": 1.6, ... }
}
```

#### `waves`

An ordered list of spawn instructions.

```json
{ "time": 20.0, "enemies": { "drone": 4, "shooter": 1 } }
```

* `time`: spawn time in seconds since level start
* `enemies`: map `{ templateId -> count }`

**Rule:** every `templateId` used in `waves[*].enemies` must exist in the top-level `enemies` dictionary.

---

## How to add more levels (scalable workflow)

### Step 1 — Create the level definition JSON

Create a new file following the same schema:

```text
assets/levels/space/space_easy_02.json
```

Give it:

* a unique `name`
* `duration`
* enemy templates
* wave schedule

### Step 2 — Register it into a catalog

Edit the catalog list:

```json
[
  {
    "id": "space_easy_01",
    "name": "Space - Tutorial Drift",
    "path": "client/assets/levels/space/space_easy_01.json"
  },
  {
    "id": "space_easy_02",
    "name": "Space - First Swarm",
    "path": "client/assets/levels/space/space_easy_02.json"
  }
]
```

### Step 3 — Add required assets

If you introduce:

* new `spriteId`s → ensure spritesheets/atlas mapping exists
* new bullet types → ensure server/client both support `shoot.type`
* new sound cues → ensure sound ids exist

### Step 4 — Validate (recommended)

At load time (client or server), validate:

* catalog entries:

    * `id`, `name`, `path` not empty
* level definition:

    * `duration > 0`
    * `waves[*].time` in `[0, duration]`
    * template ids referenced exist
    * numeric fields are in sane bounds (hp > 0, cooldown > 0, etc.)

---

## Recommended schema extensions (future-proof)

### 1) Add `version` at the root

```json
{
  "version": 1,
  "name": "...",
  ...
}
```

When you change the format later, you can:

* accept old versions
* or reject with a readable error message

### 2) Add optional wave fields

To make levels richer without breaking old ones:

```json
{
  "time": 20.0,
  "enemies": { "drone": 4 },
  "spawn": { "pattern": "line", "y": 120.0, "spacing": 40.0 }
}
```

If missing, fallback to default spawn behavior.

### 3) Split enemy templates into reusable files (optional)

If many levels reuse the same enemies:

* `assets/enemies/space.json`
* level only references them

This keeps level files shorter and reduces duplication.

---

## Suggested naming conventions

### IDs

* use lowercase + underscores
* include world + difficulty:

    * `space_easy_01`
    * `space_medium_03`

### Paths

* match the world folder:

    * `client/assets/levels/space/space_easy_01.json`

### Enemy template keys

* short logical names:

    * `"drone"`, `"shooter"`, `"boss"`

---

## Simple Diagram: Catalog → Level → Spawns

```text
┌──────────────────────────────────────┐
│Catalog: assets/levels/space/easy.json│
│  - [{ id, name, path }, ...]         │
└───────────────┬──────────────────────┘
                │ user selects level
                ▼
┌──────────────────────────────────────┐
│ Level file: space_easy_01.json       │
│  - duration                          │
│  - enemies (templates)               │
│  - waves (time → counts)             │
└───────────────┬──────────────────────┘
                │ for each wave at time T
                ▼
┌──────────────────────────────────────┐
│ Spawner / Server logic               │
│  - pick template (drone/shooter)     │
│  - spawn N enemies                   │
│  - apply shoot config                │
└──────────────────────────────────────┘
```

---

## Practical tips for “more levels” without pain

* Keep waves **coarse** (5–15 waves) and tweak counts rather than micromanaging spawns.
* Reuse enemy templates across levels for consistency.
* Use `duration` as a hard bound: no waves after duration.
* If you add bosses, treat them as just another template + a wave at a specific time.