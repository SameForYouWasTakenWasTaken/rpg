# 📖 Class Reference

A practical breakdown of the main engine and application classes in Game2 (`namespace ssg`).
For the architectural overview, see [`ARCHITECTURE.md`](ARCHITECTURE.md).

---

## 🎛️ Core

### `Engine`

A normal engine object that owns reusable engine-wide services. It is no longer a singleton.

| Member / Method | Purpose |
| --- | --- |
| `EventBus` | Central event queue and dispatch. |
| `AssetManager` | Loads and caches textures and atlases. |
| `Logger` | Structured logging and log sinks. |
| `Input` | Tracks/translates keyboard and mouse input. |
| `Renderer` | Batched 2D rendering. |
| `initialize()` | Starts the engine. |
| `terminate()` | Stops the application loop. |
| `isRunning()` | Reads the running flag. |
| `GetEventBus()` | Access the event bus. |
| `GetAssetManager()` | Access the asset manager. |
| `GetLogger()` | Access the logger. |
| `GetInputSystem()` | Access the input service. |
| `GetRenderer()` | Access the renderer. |

Copy/assignment are deleted. Multiple Engine instances are possible, although the application currently creates one in `main()`.

### `EngineContext`

A non-owning reference bundle used to pass engine dependencies explicitly.

| Member | Purpose |
| --- | --- |
| `engine` | Reference to the owning `Engine`. |
| `logger` | Convenience reference to `Engine::GetLogger()`. |
| `assetManager` | Convenience reference to `Engine::GetAssetManager()`. |

`EngineContext` does not own any of these services.

### `Entity`

A legacy placeholder. The active ECS model uses `entt::entity` IDs directly.

### `Input`

Engine-level input service.

- `IsKeyDown()` / `IsKeyUp()` query keyboard state.
- `IsMouseButtonDown()` / `IsMouseButtonUp()` query mouse state.
- `GetMousePosition()` queries mouse position.
- `ProcessEvents()` consumes raw SFML events and queues corresponding SSG input events.
- `Update(dt)` advances key-repeat timers.

---

## 🪟 App

### `Application`

Receives an `EngineContext`, owns the main `Window`, and runs the application loop.

| Method / Member | Purpose |
| --- | --- |
| `Run()` | Runs the frame loop and creates the initial scene stack. |
| `Shutdown()` | Stops the engine and closes the window. |
| `HandleEvents()` | Polls raw SFML events and routes them to input/window handling. |
| `m_EngineContext` | Reference to shared engine dependencies. |
| `m_Window` | Application-owned main window. |

The renderer is not owned by `Application`; it is owned by `Engine`.

### `ApplicationContext`

A small per-application context currently containing only `MainWindow`. It is intentionally not a general-purpose service container.

### `IScene` (abstract)

Represents a screen or game state. Owns an `entt::registry` and a list of layers.

| Member / Method | Purpose |
| --- | --- |
| `registry` | Scene-local ECS registry. |
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
| `Switch(scene)` | Replace the current scene. |
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

Current concrete `IScene` example. It installs the `GameLayer` and forwards update/render calls to its layers.

### `GameLayer`

Current concrete `ILayer` example. Receives an `EngineContext` and owns a scene-local registry plus gameplay systems.

It demonstrates player movement, camera following, entity factories, atlas loading, transforms, combat, inventory interaction, and rendering.

---

## 🧭 Application systems

### `ISystem` (abstract)

Base class for systems operating on a scene's EnTT registry.

- Stores a reference to the registry.
- Stores an `EngineContext&` for engine-wide dependencies.
- `Update(float dt)` is the per-frame entry point.

### `TransformSystem`

Derives `CWorldTransform` from local transforms and the parent/child hierarchy. Gameplay writes local transforms; the system owns world-transform derivation.

### `SpatialGrid`

A uniform spatial hash/grid for proximity queries.

- `Update(dt)` rebuilds the grid.
- `Query(pos, radius, filter)` returns nearby matching entities.
- `FindNearest(pos, radius, filter)` finds the closest matching entity.

### `CombatSystem`

Handles attack requests and timed combat resolution.

- Receives `OnAttackRequest` events.
- Tracks attack timing and hit windows in `CCombatState`.
- Uses `SpatialGrid` for nearby target queries.
- Applies `CWeapon.damage` to `CHealth`.
- Queues `OnEntityDeath` when health reaches zero.
- Prevents an entity from being hit more than once during a swing.

### `inventory`

Free functions for inventory operations. They receive `EngineContext&` when logging or factory/asset services are needed.

- `AddItem()` adds and merges item stacks.
- `FindItemIndex()` locates an item by type.
- `Equip()` / `Unequip()` manage equipment.
- `Drop()` creates a world item and removes the requested quantity.
- `RemoveItemFromInventory()` removes a quantity from an inventory stack.
- `RemoveItemFromHotbar()` is currently a placeholder.

### `hierarchy`

Free functions for modifying parent/child relationships without exposing hierarchy internals to call sites.

- `AttachChild(...)`
- `DetachChild(...)`
- `AttachMode::KeepLocal`
- `AttachMode::KeepWorld`

See [`HIERARCHY.md`](HIERARCHY.md) for the transform/hierarchy rules.

---

## 🏭 Factories

`ssg::factory` contains helpers for attaching standard component sets to an **existing** entity.

### `ApplyCharacterDefinition()`

Loads a JSON character definition, attaches its configured components, and returns the parsed JSON object. It receives an `EngineContext&` for engine services.

### `ApplyItemDefinition()`

Builds on the character definition flow and attaches `CItem` data such as item type and maximum stack size.

### `ApplyWeaponDefinition()`

Builds on the item definition flow and attaches `CWeapon` data such as damage, range, attack speed, and hit window.

The factories do not create the entity themselves; callers create an entity and pass its ID to the factory.

---

## 🖥️ Rendering

### `Renderer`

Engine-owned batched 2D renderer. `Submit()` queues render objects; `Begin()` starts a frame; `End(Window)` sorts/batches and draws them.

### `Window`

Application-owned wrapper around the SFML render window and view. It polls raw SFML events and listens for relevant window events through the event bus.

### `Camera`

Thin wrapper around `sf::View` for center, size, rotation, viewport, and relative movement/zoom.

### `Atlas`

Loads TexturePacker-style JSON metadata and maps sub-image names to texture rectangles. Atlas instances retain an `EngineContext&` so they can access the engine asset manager.

---

## 🗃️ Core systems

### `AssetManager`

Engine-owned service that loads textures and atlases and exposes them through IDs.

- `LoadTexture(path)` loads/reuses a texture.
- `GetTexture(id/path)` accesses a loaded texture.
- `LoadAtlas(context, config)` loads an atlas from an `AtlasConfig`.
- `LoadAtlas(context, path, field)` loads an atlas configuration from a named JSON field.
- `GetAtlas(id)` accesses a loaded atlas.
- `GetEntityDefinition(path)` reads an entity definition and its sprite region.

### `Logger`

Engine-owned structured logging service.

- Supports `Trace`, `Debug`, `Info`, `Warn`, `Error`, and `Fatal`.
- Convenience overloads automatically capture `std::source_location`.
- `AddSink()` registers a log destination.
- `SetLevel()` sets the global minimum log level.

The previous `Logging.hpp` macro interface has been removed in favour of direct logger access through the appropriate context.

---

## 📡 Events

### `IEvent`

Base type for SSG events.

### `EventBus`

Thin wrapper around EnTT's dispatcher.

| Method | Purpose |
| --- | --- |
| `Queue<T>(args...)` | Queue an event for later delivery. |
| `Emit<T>(args...)` | Deliver an event immediately. |
| `Update()` / `Update<T>()` | Deliver queued events. |
| `Sink<T>()` | Obtain an EnTT event sink. |
| `IsSFMLEvent<T>(event)` | Extract a typed SFML event safely. |

### Event types

Core/window, input, and gameplay events are split by responsibility. Core input/window events currently live under `src/Core/Events/`; gameplay events such as `OnAttackRequest` and `OnEntityDeath` live under `src/App/Events/Gameplay/`.

---

## 🧩 Components

Plain structs stored on scene registries.

| Component | Purpose |
| --- | --- |
| `CTransform` | Local position, scale multiplier, rotation. |
| `CWorldTransform` | Derived world position, scale, rotation. |
| `CRelationship` | Parent/child hierarchy links. |
| `CSprite` | Sprite appearance, render properties, and facing direction. |
| `CTexture` | Texture ID and source rectangle. |
| `CDefinition` | Source JSON definition path. |
| `CHealth` | Maximum and current health. |
| `CHumanoid` | Movement speed. |
| `CCombatState` | Attack state, timers, cached weapon, and hit tracking. |
| `CEquipment` | Currently equipped weapon. |
| `CItem` | Item type, current quantity, and maximum stack. |
| `CInventory` | Inventory item entities and capacity. |
| `CWeapon` | Weapon name, damage, range, attack speed, and hit window. |

> Gameplay should write `CTransform`; `TransformSystem` writes `CWorldTransform`.

---

## ⚙️ Configuration

Compile-time configuration lives under `src/Shared/Config/` and is grouped by subsystem.

- `Config::Inventory` — inventory limits and item drop distance.
- `Config::Input` — key repeat delay and interval.
- `Config::Logging` — memory sink capacity.
- `Config::Rendering` — renderer z-index layer count.

`Config.hpp` provides an aggregate include for these configuration headers.

---

## 🔤 Shared types

`src/Shared/Types.hpp` contains commonly used aliases such as `Vec2`, `Vec3`, `TextureID`, `AtlasID`, `String`, `Vector<T>`, `Array<T, N>`, and `Filepath`, along with shared inventory count/slot types.
