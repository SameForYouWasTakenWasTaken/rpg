# 🎮 Game2

> A small [SFML](https://github.com/SFML/SFML) game project written in modern C++ (C++20).
> Built around a **scene + layer** model, an **ECS** (Entity-Component-System) core,
> and a **batched renderer** for fast 2D drawing.

This is an early-stage project, but the foundations are already in place:
a render loop, an event bus, an asset manager, a sprite atlas loader, and a
working demo scene with a player you can move around with WASD.

---

## 📚 Documentation

Pick the doc you need. Each one is short and to the point.

| Doc | What it is for |
| --- | --- |
| [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) | How the pieces fit together (flow, layers, ECS). Read this first. |
| [`docs/CLASSES.md`](docs/CLASSES.md) | A plain reference of every core class and what it does. |
| [`docs/DEPENDENCIES.md`](docs/DEPENDENCIES.md) | Every third-party library and where to find it. |
| [`docs/CONTRIBUTING.md`](docs/CONTRIBUTING.md) | How to build, the code style, and where to add stuff. |

---

## 🧱 What's inside

- **Engine** — a singleton that owns the global `EventBus` and `AssetManager`.
- **Application** — owns the window, the renderer, and the main game loop.
- **Scene Stack** — a stack of `IScene` objects (like screens / states).
- **Layers** — a scene is made of `ILayer` objects (game logic, UI, etc.).
- **ECS** — entities are plain IDs; data lives in `CTransform`, `CSprite`, `CTexture`.
- **Renderer** — batches quads into one vertex buffer and draws per texture.
- **Camera** — a thin wrapper over an SFML `sf::View`.

---

## 🛠️ Build it

You need:

- A C++20 compiler (MSVC, Clang, or GCC)
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

## 🎯 Controls (current demo)

| Key | Action |
| --- | --- |
| `W` `A` `S` `D` | Move the player |
| Mouse / Wheel | Events are captured (no action yet) |
| Close window | Quits the game |

---

## 🗺️ Project layout

```
Game2/
├── CMakeLists.txt        # Top-level CMake config
├── dependencies.cmake    # Fetches SFML, GLM, EnTT, nlohmann/json
├── docs/                 # All documentation (you are here)
└── src/
    ├── main.cpp          # Entry point
    ├── Core/             # Engine, ECS, Rendering, Systems, Events
    ├── App/              # Application, Scenes, Layers, Components
    └── Shared/           # Shared types & aliases
```

---

## ✅ Status

- [x] Window + render loop
- [x] Event bus (keyboard, mouse, window)
- [x] Asset manager + texture atlas
- [x] Batched renderer
- [x] ECS with transform / sprite / texture
- [x] Demo scene with a movable player + camera
- [ ] Audio
- [ ] More scenes / UI
- [ ] Save / load

---

Made with 💙 and way too much `sf::Vertex`.
