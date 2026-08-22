# 🏗️ Architecture

This document explains how Game2 is put together. It is intended to be a practical map of the codebase rather than a complete API reference.

Everything lives in the `ssg` namespace (short for *"super secret game"* 😉).

---

## 🔄 The big picture

```text
        main()
          │
          ▼
   Engine::instance()
          │
          ├── EventBus
          ├── AssetManager
          └── Input
          │
          ▼
   Application::Run()
          │
          ├── owns Window + Renderer
          ├── builds SceneStack
          │        │
          │        └── Scene
          │             │
          │             ├── ECS registry
          │             └── Layers
          │
          ▼
   Game loop
          │
          ├── eventBus.Update()       // deliver events queued last frame
          ├── HandleEvents(window)     // poll SFML + feed Input / window events
          ├── input.Update(dt)         // input repeat/state processing
          ├── window.Clear()
          ├── renderer.Begin()
          ├── stack.Update(dt, ctx)
          ├── stack.Render(renderer, ctx)
          ├── renderer.End(window)
          └── window.Display()
```

The important ownership boundary is that **Engine owns reusable engine-wide services**, while **Application owns the game loop and application flow**. Scenes own their ECS state and layers.

---

## 🧩 Key ideas

### 1. Engine (singleton)

`Engine` is a single global object created once. It currently owns:

- `eventBus` — the central `EventBus` for queued and immediate events.
- `assetManager` — loads and caches textures.
- `inputSystem` — tracks keyboard/mouse state and converts relevant SFML input events into SSG input events.
- `m_running` — an atomic flag used to stop the application loop.

The engine deliberately contains **core services**, not game-specific state such as quests, players, inventories, or scene-transfer data.

### 2. Application (the loop owner)

`Application` owns the `Window` and `Renderer`. Its `Run()` method drives the main loop and constructs the initial `SceneStack`.

`HandleEvents()` polls raw SFML events. Input events are delegated to `Engine::inputSystem`; application/window events such as close and resize are converted directly into core events.

`ApplicationContext` is intentionally small. It currently provides the main `Window` to scenes and layers. It is not a general-purpose container for every game state or engine service.

### 3. Scene Stack

A `SceneStack` owns `std::unique_ptr<IScene>` objects. Only the **top** scene currently updates and renders.

- `Push(scene)` — add a scene on top.
- `Pop()` — remove the top scene.
- `Switch(scene)` — replace the current scene.
- `Clear()` — remove all scenes.

Scenes are application/game-flow objects. They are not engine-wide singletons.

### 4. Scenes & Layers

An `IScene` owns an `entt::registry` and a list of `ILayer` objects.

- A **scene** represents a screen or game state such as a world, menu, or pause screen.
- A **layer** is a focused slice of behaviour inside that scene.

A scene forwards update/render calls to its layers. Layers can own or use gameplay systems that operate on the scene's registry.

### 5. ECS (Entity-Component-System)

Entities are `entt::entity` IDs. Data lives in plain components attached to an EnTT registry.

| Component | Holds |
| --- | --- |
| `CTransform` | local position, scale multiplier, rotation |
| `CWorldTransform` | derived world position, scale, rotation |
| `CRelationship` | parent/child links |
| `CSprite` | color, z-index, origin, pixel size, flip flags |
| `CTexture` | texture ID + source rectangle |

The current transform convention is **write local, read world**: gameplay changes `CTransform`; `TransformSystem` derives `CWorldTransform` from the hierarchy.

### 5b. Systems

Application systems derive from `ISystem` and operate on a scene registry through `Update(dt)`.

Current systems include:

- `TransformSystem` — derives world transforms from local transforms and relationships.
- `SpatialGrid` — buckets entities into cells for proximity queries.

Entity setup is centralized in `ssg::factory`, whose helpers attach standard component sets to an existing entity.

### 6. Input

`Input` is an engine-level service because input state is not specific to one scene. It provides queries such as `IsKeyDown()` and `IsMouseButtonDown()` and also emits input events for systems that prefer event-driven handling.

The flow is:

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

This keeps gameplay code from needing to poll the raw SFML event stream.

### 7. Renderer (batched)

The `Renderer` collects `RenderObject`s through `Submit()`. Objects are grouped by z-index and sorted/batched by texture before being drawn.

The renderer is an engine facility; gameplay code submits render data but does not own the underlying SFML vertex buffers.

### 8. Events

There are two levels of events:

- **SFML events** — raw platform/window/input events returned by `Window::PollSFMLEvents()`.
- **SSG events** — project-defined events delivered through `EventBus`.

Core/window events and input events live under `src/Core/Events/`. `Input` is responsible for translating relevant SFML input events into SSG input events. `Application` handles window lifecycle events such as close and resize.

`EventBus::Queue()` delivers events on `Update()`, while `EventBus::Emit()` delivers them immediately.

### 9. Assets & Atlas

`AssetManager` loads textures once and identifies them with `TextureID` values. `Atlas` reads TexturePacker-style JSON metadata and exposes sub-texture rectangles.

---

## 📂 Folder map

| Path | Contains |
| --- | --- |
| `src/main.cpp` | Entry point |
| `src/Core/` | Engine-wide infrastructure |
| `src/Core/Rendering/` | Renderer, Window, Camera, Atlas |
| `src/Core/Systems/` | Engine systems such as AssetManager and Input |
| `src/Core/Events/` | EventBus, IEvent, window and input events |
| `src/App/` | Application, scenes, layers, and game-side infrastructure |
| `src/App/Layers/` | Concrete gameplay layers |
| `src/App/Scenes/` | Concrete scenes |
| `src/App/Components/` | ECS components |
| `src/App/Systems/` | Gameplay/ECS systems |
| `src/App/Factories/` | Entity/component setup helpers |
| `src/Shared/` | Shared type aliases |

---

## 🧭 Where to look next

- New to the code? Read [`CLASSES.md`](CLASSES.md) for the class map.
- Need hierarchy details? Read [`HIERARCHY.md`](HIERARCHY.md).
- Adding a dependency? See [`DEPENDENCIES.md`](DEPENDENCIES.md).
- Contributing or unsure where code belongs? Read [`CONTRIBUTING.md`](CONTRIBUTING.md).
