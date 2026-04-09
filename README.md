# Arcade | 
[![Architecture](https://img.shields.io/badge/Architecture-RepoLens-blue)](https://repo-lens-virid.vercel.app/share/PhilibertG/Arcade-perso)

A retro gaming platform that lets users choose and play classic arcade games through interchangeable graphical libraries, all loaded dynamically at runtime.

Built as an Epitech 2nd year project in C++20, Arcade demonstrates a plugin-based architecture where both **display renderers** and **game logic** are shared libraries (`.so`) discovered and swapped on the fly, without restarting the program.

## Features

- **Dynamic library loading:** Graphics and game modules are loaded at runtime via `dlopen`/`dlsym`, with no static dependency from the core binary.
- **Hot-swappable renderers:** Switch between nCurses and SFML (or any compatible library) while a game is running.
- **Multiple games:** Ships with Snake and Centipede; adding a new game is as simple as implementing the `IGameModule` interface and dropping a `.so` in `lib/`.
- **Score tracking:** Persistent per-user score storage across sessions.
- **Menu system:** Built-in menu to select games, graphics libraries, and enter a username.

## Architecture

```
┌──────────────────────────────┐
│         Core program         │
│  (arcade binary - no gfx)   │
└──┬────┬────┬────┬────┬──────┘
   │    │    │    │    │
   ▼    ▼    ▼    ▼    ▼
 ┌───┐┌───┐┌───┐┌────┐┌──────────┐
 │GUI││GUI││GUI││Game││  Game    │
 │ 1 ││ 2 ││ 3 ││ 1  ││   2     │
 └───┘└───┘└───┘└────┘└──────────┘
```

The core binary has **zero** graphical dependencies (`ldd` shows none). All rendering and game logic lives in shared libraries under `lib/`.

### Shared interfaces

| Interface | Role |
|---|---|
| `IDisplayModule` | Rendering, event polling, asset management |
| `IGameModule` | Game loop, simulation, entity/sound production |
| `IEntity` | Drawable element with position, size, rotation, color |

## Prerequisites

- **OS:** Linux
- **Compiler:** g++ with C++20 support
- **Build system:** CMake >= 3.20
- **Libraries:**
  - ncurses
  - SFML 2 (graphics, window, system, audio)

On Ubuntu/Debian:

```bash
sudo apt install build-essential cmake libncurses-dev libsfml-dev
```

## Build

```bash
cmake --build build/

## ou

mkdir -p build && cd build && cmake .. && make
```

This produces:
- `./arcade` : the core binary
- `./lib/arcade_ncurses.so` : nCurses display
- `./lib/arcade_sfml.so` : SFML display
- `./lib/arcade_snake.so` : Snake game
- `./lib/arcade_centipede.so` : Centipede game
- `./lib/arcade_menu.so` : Menu module

## Usage

```bash
cd ../

./build/arcade ./lib/arcade_ncurses.so
```

The argument is the path to the initial graphics library. The program opens a menu displaying available games, graphics libraries, scores, and a username input field.

### Controls

#### Global (handled by the core)

| Key | Action |
|---|---|
| `F2` | Switch to the next graphics library |
| `F3` | Switch to the next game |
| `F4` | Restart the current game |
| `Escape` | Exit the program |

#### Menu

| Key | Action |
|---|---|
| `Up` / `Down` | Navigate between menu items |
| `Left` / `Right` | Cycle through options (games, graphics libs) |
| `A`..`Z` / `Backspace` | Type or edit username |
| `Enter` | Confirm selection and launch |

#### Snake

| Key | Action |
|---|---|
| `Up` / `Down` / `Left` / `Right` | Change snake direction |

#### Centipede

| Key | Action |
|---|---|
| `Up` / `Down` / `Left` / `Right` | Move the player |
| `Space` | Shoot |

## Games

### Snake (`arcade_snake.so`)

The classic snake game. Move around the map, eat food to grow, avoid walls and your own tail. The goal is to make the snake as long as possible.

### Centipede (`arcade_centipede.so`)

Shoot descending centipedes from the bottom of the screen. When hit, a centipede splits in two. Survive 20 waves to win.

## Adding a new plugin

Arcade is fully extensible: implement `IDisplayModule` for a new graphics backend or `IGameModule` for a new game, compile as a shared library, and drop the `.so` into `lib/`.

See [Arcade_Documentation.md](Arcade_Documentation.md) for the full interface reference, entrypoints, types, and step-by-step instructions.

## Interface sharing

This project's interfaces are designed to be compatible with other Arcade implementations. To test cross-compatibility, drop external `.so` plugins into `lib/`.

## Project structure

```
.
├── assets/             # Sprites, fonts, sounds (per-game subdirectories)
├── include/            # Public interfaces (IDisplayModule, IGameModule IEntity, ...)
│   └── Core/           # Core headers (DLLoader, ScoreManager)
├── lib/                # Compiled shared libraries (.so)
├── plugins/            # Plugin source code
│   ├── Centipede/
│   ├── Menu/
│   ├── NcursesDisplay/
│   ├── SfmlDisplay/
│   └── Snake/
├── src/                # Core program source
│   ├── Core/
│   └── main.cpp
└── CMakeLists.txt
```