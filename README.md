# 🎮 Game2

> A small [SFML](https://github.com/SFML/SFML) game project written in modern C++ (C++23).
> Built around a **scene + layer** model, an **ECS** (Entity-Component-System) core,
> and a **sink-based renderer** for 2D drawing.

This is an early-stage project, but the foundations are already in place:
a render loop, an event bus, an asset manager, a sprite atlas loader, Tiled tilemap loading,
rendering sinks, and a working demo scene with a player you can move around with WASD.

---

## 📚 Documentation

Pick the doc you need. Each one is short and to the point.

| Doc | What it is for |
| --- | --- |
| [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) | How the pieces fit together (flow, ownership, layers, ECS, and rendering). Read this first. |
| [`docs/CLASSES.md`](docs/CLASSES.md) | A practical reference of the main core, application, rendering, and gameplay classes. |
| [`docs/DEPENDENCIES.md`](docs/DEPENDENCIES.md) | Every third-party library and where to find it. |
| [`docs/CONTRIBUTING.md`](docs/CONTRIBUTING.md) | How to build, the code style, ownership boundaries, and where to add stuff. |
| [`docs/TILEMAP.md`](docs/TILEMAP.md) | Tiled map/tileset formats, loading, GID resolution, and tilemap entity creation. |
| [`docs/HIERARCHY.md`](docs/HIERARCHY.md) | Transform hierarchy and parent/child rules. |

---

## 🧱 What's inside

- **Engine** — a normal object that owns reusable engine-wide services; it is not a singleton.
- **EngineContext** — a non-owning reference bundle used to pass engine services explicitly.
- **Application** — owns the window and application loop, and wires application-specific systems such as render sinks.
- **Scene Stack** — a stack of `IScene` objects (like screens / states).
- **Layers** — a scene is made of `ILayer` objects (game logic, UI, etc.).
- **ECS** — entities are plain IDs; data lives in components attached to scene-local EnTT registries.
- **Renderer** — orchestrates registered render sinks and their frame lifecycle.
- **SpriteSink** — the current application-side sink for batched sprite rendering.
- **Asset Manager** — loads and caches textures using strongly typed `TextureHandle` values.
- **Tilemap Loader** — loads Tiled `.tmj` maps with external JSON `.tsj` tilesets.
- **Data-driven configuration** — window settings are loaded from `data/game.json` instead of being hard-coded in `Application`.

---

## 🛠️ Build it

You need:

- A C++23 compiler (MSVC, Clang, or GCC)
- [CMake](https://cmake.org/) 3.28 or newer
- Git (used by CMake to fetch dependencies)

```bash
# 1. Clone / open the project
cd Game2

# 2. Make a build folder (in-source builds are blocked on purpose)
mkdir build
cd build

# 3. Configure
cmake ..

# 4. Build
cmake --build .

# 5. Run (assets are copied next to the exe automatically)
./bin/Game2        # Linux / macOS
bin\Game2.exe      # Windows
```

> 💡 Dependencies (SFML, GLM, EnTT, nlohmann/json) are fetched automatically by
> CMake through `FetchContent`. You don't need to install them by hand.
> See [`docs/DEPENDENCIES.md`](docs/DEPENDENCIES.md) for versions and links.

---

## ⚙️ Configuration

Window configuration lives in `data/game.json` and is loaded through the application factory.
The JSON contains the window title, size, framerate, VSync state, and an optional icon.

The flow is:

```text
data/game.json
      ↓
LoadWindowSettings()
      ↓
WindowSettings
      ↓
Window::SetSettings()
      ↓
Window
```

This keeps JSON parsing and configuration validation out of `Application` and `Window`.

---

## 🎯 Controls (current demo)

| Key | Action |
| --- | --- |
| `W` `A` `S` `D` | Move the player |
| Mouse / Wheel | Events are captured (no action yet) |
| Close window | Quits the game |

---

## 🗺️ Project layout

```text
Game2/
├── CMakeLists.txt        # Top-level CMake config
├── dependencies.cmake    # Fetches third-party dependencies
├── docs/                 # All documentation
└── src/
    ├── main.cpp          # Creates the Engine and Application
    ├── Core/              # Engine-wide infrastructure
    ├── App/               # Application, scenes, layers, game-side systems and assets
    └── Shared/            # Shared types, aliases and configuration
```

---

## ✅ Status

- [x] Window + render loop
- [x] Data-driven window configuration
- [x] Event bus (keyboard, mouse, window)
- [x] Asset manager + texture atlas
- [x] Texture handles
- [x] Sink-based renderer
- [x] ECS with transform / sprite / texture
- [x] Tiled `.tmj` map loading
- [x] External JSON `.tsj` tileset loading
- [x] Demo scene with a movable player + camera
- [ ] Audio
- [ ] More scenes / UI
- [ ] Save / load

---

Made with 💙 and way too much `sf::Vertex`.
