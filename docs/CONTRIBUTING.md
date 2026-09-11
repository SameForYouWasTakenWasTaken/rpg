# 🤝 Contributing

Thanks for wanting to help out! This guide is intentionally short. If you only read one thing, read **"Where to add stuff"** below.

---

## 🛠️ Building

See the build steps in the [README](../README.md#-build-it). Quick version:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./bin/Game2        # or bin\\Game2.exe on Windows
```

Dependencies are fetched by CMake when needed. Versions and setup details are in [`DEPENDENCIES.md`](DEPENDENCIES.md).

---

## 🎨 Code style

The repo ships `.clang-format` and `.clang-tidy`.

- **Standard:** C++23.
- **Namespace:** `ssg`.
- **Formatting:** run `clang-format` on files you touch.
- **Linting:** run `clang-tidy` where practical and understand any warnings you leave behind.
- **Ownership:** prefer clear ownership and RAII. Avoid globals and singletons when explicit ownership or dependency passing is practical.
- **Dependencies:** prefer passing required engine services through `EngineContext&` rather than reaching into global state.
- **Data-driven configuration:** keep runtime window configuration in `data/game.json` rather than hard-coding individual settings in `Application`.

Keep the surrounding style consistent when a tool is unavailable.

---

## 📂 Where to add stuff

| You want to add... | Put it here |
| --- | --- |
| A new screen / game state | `src/App/Scenes/` (inherit `IScene`) |
| A focused scene behaviour | `src/App/Layers/` (inherit `ILayer`) |
| An ECS component | `src/App/Components/` |
| A gameplay/ECS system | `src/App/Systems/` |
| An input event or window event | `src/Core/Events/` |
| A gameplay event | `src/App/Events/Gameplay/` |
| An engine-wide service/system | `src/Core/Systems/` |
| A generic rendering interface | `src/Core/Rendering/` |
| An application-specific rendering backend/sink | `src/App/Rendering/` |
| Entity setup helpers | `src/App/Factories/` |
| Map loading / tilemap factories | `src/App/Factories/Gameplay/Map/` |
| Compile-time configuration | `src/Shared/Config/` |
| A shared type / alias | `src/Shared/Types.hpp` |
| A game asset or runtime data file | `src/App/assets/` or `src/App/data/` |

A useful rule is: **Core provides reusable engine facilities; App contains game/application-specific behaviour.**

Do not put gameplay state in `Engine` just because many systems need access to it. Likewise, do not turn `ApplicationContext` into a catch-all bag of unrelated state.

When code needs engine-wide dependencies, prefer an `EngineContext&`. The context is non-owning; `Engine` remains the owner of the underlying services.

---

## 🧱 Common patterns

### Add a new scene
1. Create `src/App/Scenes/MyScene.hpp/.cpp` inheriting `IScene`.
2. Implement `OnUpdate` / `OnRender`.
3. Add it to the `SceneStack` where the application/game flow requires it.

### Add a new layer
1. Create `src/App/Layers/MyLayer.hpp/.cpp` inheriting `ILayer`.
2. Implement `OnAttach`, `OnDetach`, `OnUpdate`, and `OnRender`.
3. Push it into the owning scene.
4. If it needs engine services, accept an `EngineContext&` and store a reference as appropriate.

### Add an ECS component
1. Add a plain struct under `src/App/Components/`.
2. Attach it with `registry.emplace<CMyComp>(entity)`.
3. Query it through an EnTT view in the appropriate system or layer.

### Add an ECS system
If the logic operates over many entities and is naturally expressed as a query, prefer an `ISystem` under `src/App/Systems/` rather than putting the whole loop inside a layer.

Systems receive the scene registry and an `EngineContext&`. Use the context for shared services such as logging or asset access instead of introducing another global dependency.

### Create tilemap entities

Keep tilemap parsing separate from ECS entity creation. The current straightforward converter is:

```cpp
map::O_N2::CreateEntitiesForMap(registry, tilemap);
```

The `O_N2` namespace is intentional: this implementation traverses every tile and does not perform chunking or spatial partitioning. If an optimized implementation is added later, it should be distinguishable from this simple version.

### Add a render sink

Generic render-sink interfaces belong in Core. Application-specific sinks belong under `src/App/Rendering/`.

Register the sink from application setup:

```cpp
renderer.AddSink(std::make_unique<rendering::SpriteSink>());
```

`Renderer::Begin()` and `Renderer::End()` drive sink lifecycle callbacks automatically. Do not call a sink's `Begin()` or `End()` directly from gameplay/application code.

A sink may implement `HandleEvents()` when its backend genuinely needs raw SFML events. The application owns event polling and the renderer only forwards already-polled events. General gameplay input should continue to use `Input` and the `EventBus`.

### Listen to an event

For engine/core events, use the event bus exposed by the context:

```cpp
m_EngineContext.engine.GetEventBus()
    .Sink<MyEvent>()
    .connect<&MyClass::OnMyEvent>(this);
```

Queue events with `eventBus.Queue<MyEvent>(args...)`; use `Emit` only when immediate delivery is intentional.

### Log a message

Use the logger available through `EngineContext`:

```cpp
m_EngineContext.logger.Info("Category", "Something happened: {}", value);
```

The logger automatically captures the source location for the convenience overloads.

### Read input

Use the engine's Input service for state queries or input events rather than polling the raw SFML event stream from gameplay code.

```cpp
if (Input::IsKeyDown(Input::Key::W))
{
    // move
}
```

### Load a texture

Use `AssetManager::LoadTexture()` and keep the returned `TextureHandle` rather than using raw integer texture IDs:

```cpp
TextureHandle texture = context.assetManager.LoadTexture(path);
```

`TextureHandle` is the engine's strongly typed texture identifier and can represent an invalid handle explicitly.

### Add configuration

Place compile-time subsystem settings under `src/Shared/Config/` and group them by responsibility. Keep runtime window configuration in `src/App/data/game.json` and load it through `factory::LoadWindowSettings()`.

### Work with Tiled maps

Maps use Tiled's JSON `.tmj` format with external JSON `.tsj` tilesets. See [`TILEMAP.md`](TILEMAP.md) for the supported subset, path handling, GID resolution, and ECS conversion rules.

---

## 🧠 Architecture guidance

Before introducing a new global or context object, ask which object actually owns the responsibility.

- **Engine:** reusable engine-wide services such as input, assets, events, logging, and rendering.
- **EngineContext:** non-owning references used to pass engine dependencies explicitly.
- **Application:** main loop, window, event polling, and application flow.
- **Renderer:** coordinates registered rendering sinks and their lifecycle.
- **Render sink:** a concrete rendering backend or output path.
- **Scene:** scene-local ECS state and layers.
- **Layer/System:** focused gameplay behaviour.

When state needs to survive a scene transition, model that requirement explicitly rather than automatically putting every possible value into a generic context struct.

Do not add services to `EngineContext` merely because they are convenient to access. Add them when they represent a dependency that is broadly useful and belongs to `Engine` ownership; otherwise pass the narrower dependency directly.

Likewise, do not add raw SFML event processing to gameplay code just because the event is available. `Application` owns polling, `Input` translates normal gameplay input, `EventBus` distributes engine events, and `Renderer::ForwardEvent()` exists specifically for rendering backends that need backend-level SFML events.

---

## ✅ Before you open a PR

- [ ] Project builds from a clean `build/` folder.
- [ ] `clang-format` applied.
- [ ] Relevant tests/manual checks performed.
- [ ] No accidental generated/editor files included.
- [ ] New code follows the Core/App ownership boundary.
- [ ] Engine dependencies are passed explicitly where practical.
- [ ] Docs updated if architecture, dependencies, data formats, or public APIs changed.

---

## ❓ Not sure where something goes?

Read [`ARCHITECTURE.md`](ARCHITECTURE.md) for the system flow and ownership boundaries, then [`CLASSES.md`](CLASSES.md) for the class map. For tilemaps, read [`TILEMAP.md`](TILEMAP.md); for hierarchy rules, read [`HIERARCHY.md`](HIERARCHY.md).
