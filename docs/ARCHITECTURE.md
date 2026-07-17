# 🏗️ Architecture

This document explains how Game2 is put together. It is written to be both
**technical** (for devs) and **clear** (for anyone who wants to help).

Everything lives in the `ssg` namespace (short for *"super secret game"* 😉).

---

## 🔄 The big picture

```
        main()
          │
          ▼
   Engine::instance()  ── owns ──► EventBus + AssetManager
          │
          ▼
   Application::Run()
          │
          ├── creates Window + Renderer
          ├── builds a SceneStack (push GameScene)
          │        │
          │        └── GameScene holds Layers (push GameLayer)
          │                 │
          │                 └── GameLayer owns an ECS registry (entities)
          │
          ▼
   Game loop  (while window open & engine running)
          │
          ├── eventBus.Update()        // deliver queued events
          ├── HandleEvents(window)      // SFML events -> our events
          ├── window.Clear()
          ├── renderer.Begin()
          ├── stack.Update(dt, ctx)     // scenes -> layers -> systems
          ├── stack.Render(renderer)    // scenes -> layers -> submit quads
          ├── renderer.End(window)      // batch + draw to GPU
          └── window.Display()
```

---

## 🧩 Key ideas

### 1. Engine (singleton)
`Engine` is a single global object created once. It holds:

- `eventBus` — the central `EventBus` (pub/sub for game events).
- `assetManager` — loads and caches textures.
- `m_running` — an atomic flag. `Run()` stops when it becomes `false`.

### 2. Application (the loop owner)
`Application` owns the `Window` and the `Renderer`. Its `Run()` method is the
main loop. It builds the `SceneStack`, feeds SFML events into the `EventBus`,
and drives update + render every frame.

`ApplicationContext` is a small struct passed around. Right now it only holds
a reference to the main `Window`, but it is the spot to add shared services
later (audio, input, etc.).

### 3. Scene Stack
A `SceneStack` is a stack of `IScene` objects. Only the **top** scene updates
and renders. This makes it easy to model screens / states:

- `Push(scene)` — add a scene on top.
- `Pop()` — remove the top scene.
- `Switch(scene)` — pop then push (replace current screen).
- `Clear()` — remove all.

### 4. Scenes & Layers
An `IScene` owns an `entt::registry` and a list of `ILayer` objects.

- A **scene** = a "screen" (menu, game world, pause screen).
- A **layer** = a slice of logic inside a scene (player, enemies, UI).

When a scene updates, it calls `OnUpdate` on every layer. Same for render.
A layer is added with `PushLayer` (which also calls its `OnAttach`).

### 5. ECS (Entity-Component-System)
Entities are just `entt::entity` IDs. Data lives in **components**:

| Component | Holds |
| --- | --- |
| `CTransform` | **local** position, scale (multiplier), rotation |
| `CWorldTransform` | **derived** absolute position, scale, rotation |
| `CRelationship` | parent/child links (intrusive linked list) |
| `CSprite` | color, z-index, origin, pixel size, flip flags |
| `CTexture` | texture ID + source rectangle (sub-texture) |

A layer (like `GameLayer`) creates entities, attaches these components, and
reads them in `OnUpdate` / `OnRender`.

> **Write local, read world.** Gameplay edits `CTransform`; everything visual /
> spatial reads `CWorldTransform`. Exactly one system (`TransformSystem`) derives
> world from local each frame. See [`HIERARCHY.md`](HIERARCHY.md) for the full
> pipeline.

### 5b. Systems
Systems derive from `ISystem` and operate on the registry once per frame via
`Update(dt)`:

| System | Purpose |
| --- | --- |
| `TransformSystem` | Derives `CWorldTransform` from `CTransform` + hierarchy, parents first. |
| `SpatialGrid` | Buckets entities by cell for "what is near this point?" queries. |

Entities are set up through **factories** (`ssg::factory`) that attach a standard
set of components (transform pair + sprite/texture) to an existing entity, so the
local + world transform pair always exists together.

### 6. Renderer (batched)
The `Renderer` collects `RenderObject`s through `Submit()`. Each object is
sorted into a **z-index layer**. At `End()`, it:

1. Sorts each layer by texture.
2. Groups objects that share a texture into a **batch**.
3. Writes quads into one `sf::VertexBuffer` and draws the batch.

This means many sprites = few draw calls. 🚀

### 7. Events
Two event worlds exist:

- **SFML events** — raw OS/window events from `window.PollSFMLEvents()`.
- **SSG events** — our own event types (e.g. `KeyPressedEvent`) that derive
  from `IEvent`.

`Application::HandleEvents` translates SFML events into SSG events and queues
them on the `EventBus`. Listeners (like `Window` or `GameLayer`) `connect` a
sink and get called when the bus is updated.

### 8. Assets & Atlas
`AssetManager` loads a texture once and gives it a `TextureID`.
`Atlas` reads a sprite-sheet `.json` (in the TexturePacker format) and maps
each sub-image name (e.g. `dogbite.jpg`) to its rectangle in the sheet.
Layered code asks the atlas for a sub-texture rect, then renders it.

---

## 📂 Folder map

| Path | Contains |
| --- | --- |
| `src/main.cpp` | Entry point |
| `src/Core/` | Engine, Entity, Rendering, Systems, Events |
| `src/Core/Rendering/` | `Renderer`, `Window`, `Camera`, `Atlas` |
| `src/Core/Systems/` | `AssetManager` |
| `src/Core/Events/` | `EventBus`, `IEvent`, window events |
| `src/App/` | `Application`, `Scene`, `SceneStack`, `ILayer` |
| `src/App/Layers/` | `GameLayer` (example layer) |
| `src/App/Scenes/` | `GameScene` (example scene) |
| `src/App/Components/` | `CTransform`, `CWorldTransform`, `CRelationship`, `CSprite`, `CTexture` |
| `src/App/Systems/` | `ISystem`, `TransformSystem`, `SpatialGrid`, `Hierarchy` |
| `src/App/Factories/` | `Default` (`AddDefault*` entity setup) |
| `src/App/Events/` | Input events (key, mouse, text) |
| `src/Shared/` | Shared type aliases (`Types.hpp`) |

---

## ➡️ Where to go next

- New to the code? Read [`CLASSES.md`](CLASSES.md) for a per-class breakdown.
- Adding a dependency? See [`DEPENDENCIES.md`](DEPENDENCIES.md).
- Want to help? Read [`CONTRIBUTING.md`](CONTRIBUTING.md).
