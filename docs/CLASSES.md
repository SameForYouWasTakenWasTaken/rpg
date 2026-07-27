# 📖 Class Reference

A plain breakdown of every core class in Game2 (namespace `ssg`).
For the "how it fits" view, see [`ARCHITECTURE.md`](ARCHITECTURE.md).

---

## 🎛️ Core

### `Engine`
The single global object. Created once via `Engine::instance()`.

| Member | Type | Purpose |
| --- | --- | --- |
| `eventBus` | `EventBus` | Central pub/sub for game events. |
| `assetManager` | `AssetManager` | Loads and caches textures. |
| `initialize()` | `void` | Sets `m_running = true`. |
| `terminate()` | `void` | Sets `m_running = false` (stops the loop). |
| `isRunning()` | `bool` | Reads the running flag (atomic, thread-safe). |

> 📌 Copy/assign are deleted. It is a true singleton.

---

### `Entity`
A placeholder class. Currently empty (default copy/move). The real ECS work is
done with `entt::entity` IDs inside layers, not this type. 

> ⚠️ Most likely being deprecated!
---

## 🪟 App

### `Application`
Owns the `Window` and `Renderer` and runs the main loop.

| Member | Purpose |
| --- | --- |
| `Run()` | Sets up window, builds the scene stack, runs the game loop. |
| `Shutdown()` | Stops the engine and closes the window. |
| `HandleEvents(Window&)` | Polls SFML events and queues SSG events on the bus. |
| `ApplicationContext` | Small struct holding `MainWindow&`. Passed to scenes/layers. |

---

### `IScene` (abstract)
A screen/state. Holds an `entt::registry` and a list of layers.

| Member | Purpose |
| --- | --- |
| `registry` | The ECS registry for this scene. |
| `OnUpdate(dt, ctx)` | Update hook (calls each layer). |
| `OnRender(renderer, ctx)` | Render hook (calls each layer). |
| `PushLayer(layer)` | Add a layer and call its `OnAttach()`. |
| `PopLayer(layer)` | Remove the top layer and call its `OnDetach()`. |

> Copy/assign/move are deleted.

---

### `SceneStack`
A stack of scenes. Only the **top** scene updates and renders.

| Method | Purpose |
| --- | --- |
| `Push(scene)` | Add a scene on top. |
| `Pop()` | Remove and return the top scene. |
| `Switch(scene)` | Replace the top scene (pop + push). |
| `Clear()` | Remove all scenes. |
| `Empty()` | `true` if no scenes. |
| `Update(dt, ctx)` | Update the top scene. |
| `Render(renderer, ctx)` | Render the top scene. |

---

### `ILayer` (abstract)
A slice of logic inside a scene.

| Method | Purpose |
| --- | --- |
| `OnAttach()` | Called when added to a scene. |
| `OnDetach()` | Called when removed. |
| `OnUpdate(dt, ctx)` | Per-frame logic. |
| `OnRender(renderer, ctx)` | Per-frame drawing. |

> Copy/assign/move are deleted.

---

### `GameScene` (example `IScene`)
A concrete scene. Its constructor pushes a `GameLayer`. `OnUpdate` /
`OnRender` forward to every layer.

---

### `GameLayer` (example `ILayer`)
A concrete layer. It:

- Loads a texture atlas in `OnAttach()`.
- Creates a few demo entities (`CTransform` + `CSprite` + `CTexture`).
- Moves `m_LocalPlayer` with WASD in `OnUpdate`.
- Follows the player with `m_LocalPlayerCamera`.
- Submits each entity as a `RenderObject` in `OnRender`.

| Member | Purpose |
| --- | --- |
| `m_Registry` | Local ECS registry for this layer. |
| `m_SpatialGrid` | `SpatialGrid` system bound to the registry. |
| `m_TransformSystem` | `TransformSystem` that derives world transforms. |
| `m_LocalPlayer` | The player entity. |
| `m_LocalPlayerCamera` | Camera that follows the player. |
| `OnWindowResize(e)` | Keeps the camera size in sync with the window. |

---

## 🧭 App Systems (`src/App/Systems/`)

Gameplay-side systems that run over the scene's registry. For the full transform
& hierarchy story see [`HIERARCHY.md`](HIERARCHY.md).

### `ISystem` (abstract)
Base class for systems (mirrors `ILayer`).

| Member | Purpose |
| --- | --- |
| `ISystem(registry&)` | Bind the system to a registry. |
| `Update(float dt)` | Per-frame entry point (pure virtual). |
| `m_Registry` | The registry the system operates on (protected). |

### `TransformSystem`
Derives every `CWorldTransform` from `CTransform` + the `CRelationship` chain,
parents before children.

| Method | Purpose |
| --- | --- |
| `Update(dt)` | Walk each root recursively and fill `CWorldTransform`. |

> Composition: `world.scale = parent.scale * local.scale`,
> `world.rotation = parent.rotation + local.rotation`,
> `world.position = parent.position + parent.scale * local.position`.

### `SpatialGrid`
A uniform grid (`std::unordered_map<GridKey, ...>`) for proximity queries,
rebuilt each frame.

| Method | Purpose |
| --- | --- |
| `Update(dt)` | `ISystem` hook; calls `Rebuild()`. |
| `Rebuild()` | Clear and re-bucket every entity by cell. |
| `Insert(entity)` | Add one entity to its cell. |
| `Clear()` | Empty the grid. |
| `Query(pos, radius, filter)` | Entities within `radius` passing `filter`. |
| `FindNearest(pos, radius, filter)` | Closest matching entity (or `entt::null`). |

### `hierarchy` (free functions, `ssg::hierarchy`)
Parent/child linked-list surgery. Call sites never touch the links directly.

| Function | Purpose |
| --- | --- |
| `AttachChild(reg, parent, child)` | Pure hierarchy attach (no transforms). |
| `AttachChild(reg, parent, child, AttachMode)` | Attach, then apply mode behaviour. |
| `DetachChild(reg, child)` | Unlink a child (turn it back into a root). |
| `AttachMode` | `KeepLocal` (default) / `KeepWorld` (rebase local, no teleport). |

---

## 🏭 Factories (`src/App/Factories/`, `ssg::factory`)

Attach a standard set of components to an **existing** entity (they don't create
it). Guarantees the local + world transform pair always exists together.

| Function | Adds |
| --- | --- |
| `AddDefaultTransform(reg, e)` | `CTransform` + `CWorldTransform` + `CRelationship` (root) |
| `AddDefaultTexture(reg, e)` | the above + an empty `CTexture` |
| `AddDefaultSprite(reg, e, texID, rect, size={100,100})` | the above + `CSprite` + `CTexture` |

---

## 🖥️ Rendering

### `Renderer`
Batched 2D renderer built on SFML vertex buffers.

| Member / Method | Purpose |
| --- | --- |
| `Submit(obj)` | Queue a `RenderObject` into its z-index layer. |
| `Begin()` | Clear all layers for a new frame. |
| `End(window)` | Sort, batch by texture, and draw to the window. |
| `RenderObject` | A quad: pos, scale, origin, rotation, z-index, color, texRect, texture. |

> Internally: `MAX_LAYERS` = 256 (one per `zIndex_t` value). Each layer is
> sorted by texture; consecutive same-texture objects are drawn in one batch.

---

### `Window`
Wraps an `sf::RenderWindow` and keeps an `sf::View` in sync.

| Method | Purpose |
| --- | --- |
| `SetSettings(settings)` | Apply size, title, framerate. |
| `SetTitle / SetFramerate / SetSize` | Individual setters. |
| `SetView(view)` | Apply an `sf::View`. |
| `Draw(...)` | Forward to the SFML window draw. |
| `Display()` / `Clear(color)` | Frame end / start. |
| `PollSFMLEvents()` | Return the next raw SFML event (or `nullopt`). |
| `UpdateView(func)` | Mutate the internal view, then apply it. |

`WindowSettings`: `Width`, `Height`, `Framerate`, `title`.

> The window listens to `WindowResizeEvent` and `WindowCloseEvent` on the bus.

---

### `Camera`
Thin wrapper over `sf::View`.

| Method | Purpose |
| --- | --- |
| `SetCenter / GetCenter` | Where the camera looks. |
| `SetSize / GetSize` | View size (world units). |
| `SetRotation / GetRotation` | View rotation (degrees). |
| `SetViewport / GetViewport` | Screen rectangle the view fills. |
| `Move / Rotate / Zoom` | Relative changes. |
| `SetView(center, size)` | Set both at once. |
| `GetView()` | The underlying `sf::View`. |

---

### `Atlas`
Loads a sprite-sheet `.json` + `.png` and maps sub-image names to rects.

| Method | Purpose |
| --- | --- |
| `LoadTexture(json, png)` | Parse the atlas JSON, load the texture, return a `TextureID`. |
| `GetSubTextureDimensions(name)` | Get the rect for one sub-image. |
| `GetAllSubTextureDimensions()` | Get the whole name→rect map. |

> Expects TexturePacker-style JSON with a `frames` array.

---

## 🗃️ Systems

### `AssetManager`
Loads textures once and hands them out by `TextureID` or path.

| Method | Purpose |
| --- | --- |
| `LoadTexture(path)` | Load (or reuse) a texture; returns its `TextureID`. |
| `GetTexture(id)` | Get a loaded texture by ID. |
| `GetTexture(path)` | Get a loaded texture by file path. |

> Copy/assign/move are deleted.

---

## 📡 Events

### `IEvent` (abstract)
Base class for all SSG events. Each event type gets a unique static ID via
the `GENERATE_EVENT_TYPE()` macro.

### `EventBus`
Pub/sub wrapper around `entt::dispatcher`.

| Method | Purpose |
| --- | --- |
| `Queue<T>(args...)` | Enqueue an event (delivered on `Update`). |
| `Emit<T>(args...)` | Fire an event immediately. |
| `Update()` / `Update<T>()` | Deliver queued events (all or one type). |
| `Sink<T>()` | Get a sink to `connect` a listener. |
| `IsSFMLEvent<T>(event)` | Helper to read a typed SFML event safely. |

> Copy/assign/move are deleted.

### Event types
- **Core (`src/Core/Events/`)**: `WindowCloseEvent`, `WindowResizeEvent`, `WindowSetView`.
- **Input (`src/App/Events/`)**: `KeyPressedEvent`, `KeyReleasedEvent`,
  `MouseButtonPressedEvent`, `MouseButtonReleasedEvent`, `MouseMovedEvent`,
  `MouseWheelScrolledEvent`, `TextEnteredEvent`.

---

## 🧩 Components (`src/App/Components/`)

Plain structs stored on entities.

| Component | Fields |
| --- | --- |
| `CTransform` | **local** `position` (Vec2), `scale` (Vec2, a *multiplier*), `rotation` (float) |
| `CWorldTransform` | **derived** `position` (Vec2), `scale` (Vec2), `rotation` (float) — written only by `TransformSystem` |
| `CRelationship` | `children` (size_t), `first`/`prev`/`next`/`parent` (entt::entity) — intrusive linked-list hierarchy |
| `CSprite` | `color` (sf::Color), `zIndex` (uint8), `origin` (Vec2, normalized), `size` (Vec2, pixels), `flipX`, `flipY` |
| `CTexture` | `textureID` (TextureID), `textureRect` (sf::FloatRect) |

> `CTransform.scale` is a multiplier (default `{1,1}`); the pixel size lives on
> `CSprite.size`. See [`HIERARCHY.md`](HIERARCHY.md) for the transform pipeline.

---

## 🔤 Shared types (`src/Shared/Types.hpp`)

Handy aliases used everywhere:

| Alias | Maps to |
| --- | --- |
| `Vec2` / `Vec3` | `glm::vec2` / `glm::vec3` |
| `zIndex_t` | `std::uint8_t` |
| `TextureID` | `std::uint32_t` |
| `String` | `std::string` |
| `Vector<T>` | `std::vector<T>` |
| `Array<T, N>` | `std::array<T, N>` |
| `Filepath` | `std::filesystem::path` |
