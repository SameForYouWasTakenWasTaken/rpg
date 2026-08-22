# 📖 Class Reference

A practical breakdown of the main engine and application classes in Game2 (`namespace ssg`).
For the architectural overview, see [`ARCHITECTURE.md`](ARCHITECTURE.md).

---

## 🎛️ Core

### `Engine`
The single global engine object, created through `Engine::instance()`.

| Member | Type | Purpose |
| --- | --- | --- |
| `eventBus` | `EventBus` | Central event dispatch/queue. |
| `assetManager` | `AssetManager` | Loads and caches textures. |
| `inputSystem` | `Input` | Tracks input state and translates SFML input events. |
| `initialize()` | `void` | Starts the engine. |
| `terminate()` | `void` | Stops the application loop. |
| `isRunning()` | `bool` | Reads the running flag. |

> Copy/assignment are deleted; this is a true singleton.

### `Entity`
A legacy placeholder. The actual ECS currently uses `entt::entity` IDs directly. This class is not part of the active ECS model and may be removed.

### `Input`
Engine-level input service.

- `IsKeyDown()` / `IsKeyUp()` query keyboard state.
- `IsMouseButtonDown()` / `IsMouseButtonUp()` query mouse state.
- `GetMousePosition()` queries mouse position.
- `ProcessEvents()` consumes raw SFML events and updates input state / queues input events.
- `Update(dt)` advances input repeat timers.

The implementation currently uses SFML key and mouse types directly.

---

## 🪟 App

### `Application`
Owns the `Window` and `Renderer` and runs the main loop.

| Method / Member | Purpose |
| --- | --- |
| `Run()` | Sets up the application and runs the frame loop. |
| `Shutdown()` | Stops the engine and closes the window. |
| `HandleEvents(Window&)` | Polls raw SFML events and routes them to input/window handling. |
| `ApplicationContext` | Small context containing the main `Window`. |

### `IScene` (abstract)
Represents a screen/game state. Owns an `entt::registry` and a list of layers.

| Member | Purpose |
| --- | --- |
| `registry` | ECS registry associated with the scene. |
| `OnUpdate(dt, ctx)` | Update hook. |
| `OnRender(renderer, ctx)` | Render hook. |
| `PushLayer(layer)` | Adds a layer and attaches it. |
| `PopLayer(...)` | Removes a layer and detaches it. |

### `SceneStack`
Owns the application's scenes. Only the top scene currently updates and renders.

| Method | Purpose |
| --- | --- |
| `Push(scene)` | Add a scene. |
| `Pop()` | Remove and return the top scene. |
| `Switch(scene)` | Replace the top scene. |
| `Clear()` | Remove all scenes. |
| `Empty()` | Test whether the stack is empty. |
| `Update(dt, ctx)` | Update the top scene. |
| `Render(renderer, ctx)` | Render the top scene. |

### `ILayer` (abstract)
A focused slice of logic inside a scene.

| Method | Purpose |
| --- | --- |
| `OnAttach()` | Called when added to a scene. |
| `OnDetach()` | Called when removed. |
| `OnUpdate(dt, ctx)` | Per-frame logic. |
| `OnRender(renderer, ctx)` | Per-frame rendering. |

### `GameScene`
Current concrete `IScene` example. It installs a `GameLayer` and forwards update/render calls to its layers.

### `GameLayer`
Current concrete `ILayer` example. It demonstrates player movement, camera following, ECS entities, atlas usage, transforms, and rendering.

---

## 🧭 Application systems

### `ISystem` (abstract)
Base class for systems operating on an EnTT registry.

- `ISystem(registry&)` binds the system to a registry.
- `Update(float dt)` is the per-frame entry point.

### `TransformSystem`
Derives `CWorldTransform` from local transforms and the parent/child hierarchy.

### `SpatialGrid`
A uniform spatial hash/grid for proximity queries.

- `Update(dt)` rebuilds the grid.
- `Query(pos, radius, filter)` returns nearby matching entities.
- `FindNearest(pos, radius, filter)` finds the closest matching entity.

### `hierarchy`
Free functions for modifying parent/child relationships without exposing linked-list surgery to call sites.

- `AttachChild(...)`
- `DetachChild(...)`
- `AttachMode::KeepLocal`
- `AttachMode::KeepWorld`

See [`HIERARCHY.md`](HIERARCHY.md) for the transform/hierarchy rules.

---

## 🏭 Factories

`ssg::factory` contains helpers for attaching standard component sets to an **existing** entity.

- `AddDefaultTransform()`
- `AddDefaultTexture()`
- `AddDefaultSprite()`

These establish the local/world transform pair and optional rendering components.

---

## 🖥️ Rendering

### `Renderer`
Batched 2D renderer. `Submit()` queues render objects; `Begin()` starts a frame; `End()` sorts/batches and draws them.

### `Window`
Wraps the SFML render window and view. It also exposes raw SFML event polling and listens for relevant window events.

### `Camera`
Thin wrapper around `sf::View` for center, size, rotation, viewport, and relative movement/zoom.

### `Atlas`
Loads TexturePacker-style JSON metadata plus a PNG texture and maps sub-image names to texture rectangles.

---

## 🗃️ Core systems

### `AssetManager`
Loads textures once and exposes them by `TextureID` or path.

---

## 📡 Events

### `IEvent`
Base type for SSG events.

### `EventBus`
Thin wrapper around `entt::dispatcher`.

| Method | Purpose |
| --- | --- |
| `Queue<T>(args...)` | Queue an event for later delivery. |
| `Emit<T>(args...)` | Deliver an event immediately. |
| `Update()` / `Update<T>()` | Deliver queued events. |
| `Sink<T>()` | Obtain an EnTT event sink. |
| `IsSFMLEvent<T>(event)` | Extract a typed SFML event safely. |

### Event types
Core/window and input events currently live under `src/Core/Events/`, including:

- `WindowCloseEvent`
- `WindowResizeEvent`
- `WindowSetView`
- `KeyPressedEvent`
- `KeyReleasedEvent`
- `MouseButtonPressedEvent`
- `MouseButtonReleasedEvent`
- `MouseMovedEvent`
- `MouseWheelScrolledEvent`
- `TextEnteredEvent`

---

## 🧩 Components

Plain structs stored on scene registries.

| Component | Purpose |
| --- | --- |
| `CTransform` | Local position, scale multiplier, rotation. |
| `CWorldTransform` | Derived world position, scale, rotation. |
| `CRelationship` | Parent/child hierarchy links. |
| `CSprite` | Sprite appearance and render properties. |
| `CTexture` | Texture ID and source rectangle. |

> Gameplay should write `CTransform`; `TransformSystem` writes `CWorldTransform`.

---

## 🔤 Shared types

`src/Shared/Types.hpp` contains commonly used aliases such as `Vec2`, `Vec3`, `TextureID`, `String`, `Vector<T>`, `Array<T, N>`, and `Filepath`.
