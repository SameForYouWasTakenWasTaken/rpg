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
- **Ownership:** prefer clear ownership and RAII. Do not make objects global merely because they are convenient to access.

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
| An engine-wide service/system | `src/Core/Systems/` |
| A rendering feature | `src/Core/Rendering/` |
| Entity setup helpers | `src/App/Factories/` |
| A shared type / alias | `src/Shared/Types.hpp` |
| A game asset | `src/App/assets/` |

A useful rule is: **Core provides reusable engine facilities; App contains game/application-specific behaviour.**

Do not put gameplay state in `Engine` just because many systems need access to it. Likewise, do not turn `ApplicationContext` into a catch-all bag of unrelated state.

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

### Add an ECS component
1. Add a plain struct under `src/App/Components/`.
2. Attach it with `registry.emplace<CMyComp>(entity)`.
3. Query it through an EnTT view in the appropriate system or layer.

### Add an ECS system
If the logic operates over many entities and is naturally expressed as a query, prefer an `ISystem` under `src/App/Systems/` rather than putting the whole loop inside a layer.

### Listen to an event
```cpp
Engine::instance().eventBus.Sink<MyEvent>().connect<&MyClass::OnMyEvent>(this);
```

Queue events with `eventBus.Queue<MyEvent>(args...)`; use `Emit` only when immediate delivery is intentional.

### Read input
Use `Engine::instance().inputSystem` for state queries or input events rather than polling the raw SFML event stream from gameplay code.

---

## 🧠 Architecture guidance

Before introducing a new global or context object, ask which object actually owns the responsibility.

- **Engine:** reusable engine-wide services such as input, assets, and events.
- **Application:** main loop, window, renderer, and application flow.
- **Scene:** scene-local ECS state and layers.
- **Layer/System:** focused gameplay behaviour.

When state needs to survive a scene transition, model that requirement explicitly rather than automatically putting every possible value into a generic transfer/context struct.

---

## ✅ Before you open a PR

- [ ] Project builds from a clean `build/` folder.
- [ ] `clang-format` applied.
- [ ] Relevant tests/manual checks performed.
- [ ] No accidental generated/editor files included.
- [ ] New code follows the Core/App ownership boundary.
- [ ] Docs updated if architecture, dependencies, or public APIs changed.

---

## ❓ Not sure where something goes?

Read [`ARCHITECTURE.md`](ARCHITECTURE.md) for the system flow and ownership boundaries, then [`CLASSES.md`](CLASSES.md) for the class map. For hierarchy rules, read [`HIERARCHY.md`](HIERARCHY.md).
