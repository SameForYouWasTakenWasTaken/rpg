# 🏗️ Architecture

This document explains how Game2 is put together. It is intended to be a practical map of the codebase rather than a complete API reference.

Everything lives in the `ssg` namespace (short for *"super secret game"* 😉).

---

## 🔄 The big picture

```text
                         main()
                           │
                           ▼
                    ┌─────────────┐
                    │   Engine    │
                    │             │
                    │ EventBus    │
                    │ AssetManager│
                    │ Logger      │
                    │ Input       │
                    │ Renderer    │
                    └──────┬──────┘
                           │
                    EngineContext
                           │
                           ▼
                    Application
                           │
                    owns Window
                           │
                           ▼
                    SceneStack
                           │
                           ▼
                        Scene
                     ┌─────┴─────┐
                     │           │
                ECS registry   Layers
                     │           │
                     └─────┬─────┘
                           │
                    Gameplay systems
```

The application frame loop currently follows this order:

```text
EventBus::Update()
      │
      ▼
HandleEvents() ──► Input::ProcessEvents()
      │             └── queues input events
      ├── queue window events
      └── queue text events
      │
      ▼
Window::Clear()
      │
      ▼
Renderer::Begin()
      │
      ▼
SceneStack::Update()
      │
      ▼
SceneStack::Render()
      │
      ▼
Renderer::End(Window)
      │
      ▼
Window::Display()
```

The important ownership boundary is that **Engine owns reusable engine-wide services**, while **Application owns the application flow and its Window**. The `Renderer` is currently an Engine-owned service. Scenes own their ECS state and layers.

---

## 🧩 Key ideas

### 1. Engine

`Engine` is now a normal object rather than a singleton. `main()` creates an `Engine`, initializes it, creates an `EngineContext`, and passes that context into the application.

The engine owns reusable services:

- `EventBus` — central event queue and dispatch.
- `AssetManager` — loads and caches textures and atlases.
- `Logger` — structured logging and log sinks.
- `Input` — keyboard/mouse input handling and input event translation.
- `Renderer` — batched 2D rendering.
- `m_running` — controls whether the application loop should continue.

Engine should contain reusable infrastructure, not game-specific state such as players, quests, inventories, or scene-transfer data.

### 2. EngineContext

`EngineContext` is a lightweight reference bundle passed to code that needs access to engine services without relying on global state.

It contains:

- `engine` — the owning `Engine` instance.
- `logger` — convenience reference to the engine logger.
- `assetManager` — convenience reference to the engine asset manager.

The context does not duplicate ownership. All members are references to objects owned elsewhere.

Systems, layers, and factories that need engine services should receive an `EngineContext&` rather than reaching for a global Engine instance.

### 3. Application (the loop owner)

`Application` receives an `EngineContext&` and owns the main `Window`. It uses the engine context to access the event bus, renderer, input system, and engine lifetime state.

`HandleEvents()` polls raw SFML events. Input-related events are delegated to `Input::ProcessEvents()`, while close, resize, and text events are converted into SSG events directly.

`ApplicationContext` remains intentionally small and currently contains only the main `Window`. It is not a general-purpose service container.

### 4. Scene Stack

A `SceneStack` owns `std::unique_ptr<IScene>` objects. Only the **top** scene currently updates and renders.

- `Push(scene)` — add a scene on top.
- `Pop()` — remove the top scene.
- `Switch(scene)` — replace the current scene.
- `Clear()` — remove all scenes.

Scenes are application/game-flow objects and are not global engine services.

### 5. Scenes & Layers

An `IScene` owns an `entt::registry` and a list of `ILayer` objects.

- A **scene** represents a screen or game state such as a world, menu, or pause screen.
- A **layer** is a focused slice of behaviour inside that scene.

A scene forwards update/render calls to its layers. Layers can own gameplay systems that operate on the scene's registry.

`GameLayer` receives the shared `EngineContext` and passes it to its systems, including `SpatialGrid`, `TransformSystem`, and `CombatSystem`.

### 6. ECS (Entity-Component-System)

Entities are `entt::entity` IDs. Data lives in plain components attached to a scene's EnTT registry.

| Component | Holds |
| --- | --- |
| `CTransform` | local position, scale multiplier, rotation |
| `CWorldTransform` | derived world position, scale, rotation |
| `CRelationship` | parent/child links |
| `CSprite` | color, z-index, origin, pixel size, flip flags, facing direction |
| `CTexture` | texture ID + source rectangle |
| `CHealth` | maximum and current health |
| `CHumanoid` | movement speed |
| `CCombatState` | attack state, timing, cached weapon, already-hit entities |
| `CEquipment` | currently equipped weapon |
| `CItem` | item type, current quantity, maximum stack |
| `CInventory` | inventory item entities and capacity |
| `CWeapon` | weapon damage, range, attack speed, and hit window |

The transform convention is **write local, read world**: gameplay changes `CTransform`; `TransformSystem` derives `CWorldTransform` from the hierarchy.

### 7. Systems

Systems derive from `ISystem` and operate on a scene registry. They also receive an `EngineContext&` when they need engine-wide services.

Current application systems include:

- `TransformSystem` — derives world transforms from local transforms and relationships.
- `SpatialGrid` — buckets entities into cells for proximity queries.
- `CombatSystem` — handles attack requests, timed hit windows, damage, and death events.
- Inventory functionality in `inventory` — manages item insertion, stacking, equipment, and dropping.

`ISystem` stores references to both the registry it operates on and the shared `EngineContext`.

Entity setup is centralized in `ssg::factory`, whose helpers attach standard component sets to an existing entity.

### 8. Input

`Input` is an engine-owned service. It provides state queries such as `IsKeyDown()` and `IsMouseButtonDown()` and translates relevant SFML input events into SSG input events.

The event path is:

```text
SFML event
    │
    ▼
Application::HandleEvents()
    │
    ▼
Input::ProcessEvents()
    │
    ├── update input state
    └── queue SSG input event
             │
             ▼
         EventBus
```

The input class also contains repeat-timer processing in `Update(dt)`. The current application loop does not call `Input::Update()` yet, so repeat processing is available in the service but is not currently advanced by the main loop.

Gameplay code should use the Input API rather than reading raw SFML input events directly.

### 9. Renderer

The `Renderer` is owned by `Engine` and exposed through `Engine::GetRenderer()`.

It collects `RenderObject`s through `Submit()`. Objects are organized by z-index and sorted/batched by texture before being drawn. `Begin()` starts a frame and `End(Window)` submits the batched geometry to the window.

Gameplay code submits render data but does not own the renderer's underlying SFML vertex buffers.

### 10. Events

There are two levels of events:

- **SFML events** — raw platform/window/input events returned by `Window::PollSFMLEvents()`.
- **SSG events** — project-defined events delivered through `EventBus`.

Core/window and input events live under `src/Core/Events/`. `Input` translates keyboard and mouse events into SSG input events. `Application` handles window lifecycle and text events.

`EventBus::Queue()` delivers events on a later `Update()`, while `EventBus::Emit()` delivers an event immediately.

### 11. Assets & Atlas

`AssetManager` loads textures once and identifies them with `TextureID` values. It also loads atlases from configuration or a named field in an atlas JSON file.

`Atlas` reads TexturePacker-style metadata and maps sub-image names to texture rectangles. Atlas loading receives an `EngineContext&` so it can access the owning asset manager without global engine state.

### 12. Logging

`Logger` is an engine-owned service. Gameplay and engine code can access it through `EngineContext::logger`.

The logger supports `Trace`, `Debug`, `Info`, `Warn`, `Error`, and `Fatal` levels. Convenience overloads automatically capture `std::source_location`, while explicit source locations remain available when needed.

The old global logging macros and `Logging.hpp` have been removed; code now calls the logger directly through an appropriate context/reference.

### 13. Configuration

Compile-time configuration values are grouped under `ssg::Config` in `src/Shared/Config/`.

Configuration is split by subsystem, including:

- `Config::Inventory` — inventory limits and item drop distance.
- `Config::Input` — key repeat timing.
- `Config::Logging` — memory log capacity.
- `Config::Rendering` — renderer layer count.

`Config.hpp` provides an aggregate include while individual configuration headers can be included when only one subsystem's settings are needed.

---

## 📂 Folder map

| Path | Contains |
| --- | --- |
| `src/main.cpp` | Engine/application entry point |
| `src/Core/` | Engine-wide infrastructure |
| `src/Core/Rendering/` | Renderer, Window, Camera, Atlas |
| `src/Core/Systems/` | AssetManager and Input |
| `src/Core/Events/` | EventBus, IEvent, window and input events |
| `src/App/` | Application, scenes, layers, and game-side infrastructure |
| `src/App/Layers/` | Concrete gameplay layers |
| `src/App/Scenes/` | Concrete scenes |
| `src/App/Components/` | ECS components |
| `src/App/Systems/` | Gameplay/ECS systems |
| `src/App/Factories/` | Entity/component setup helpers |
| `src/Shared/Config/` | Compile-time subsystem configuration |
| `src/Shared/` | Shared type aliases and utilities |

---

## 🧭 Where to look next

- New to the code? Read [`CLASSES.md`](CLASSES.md) for the class map.
- Need hierarchy details? Read [`HIERARCHY.md`](HIERARCHY.md).
- Adding a dependency? See [`DEPENDENCIES.md`](DEPENDENCIES.md).
- Contributing or unsure where code belongs? Read [`CONTRIBUTING.md`](CONTRIBUTING.md).
