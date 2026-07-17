# 🤝 Contributing

Thanks for wanting to help out! This guide is short on purpose. If you only
read one thing, read **"Where to add stuff"** below.

---

## 🛠️ Building

See the build steps in the [README](../README.md#-build-it). Quick version:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./bin/Game2        # or bin\Game2.exe on Windows
```

Dependencies are fetched for you by CMake. Versions & links are in
[`DEPENDENCIES.md`](DEPENDENCIES.md).

---

## 🎨 Code style

The repo ships `.clang-format` and `.clang-tidy`. Use them.

- **Standard:** C++23 (`CMAKE_CXX_STANDARD 23`).
- **Namespace:** everything is in `namespace ssg`.
- **Classes:** copy/assign/move are deleted on most core types (singletons,
  renderers, managers). Follow that pattern for non-copyable objects.
- **Format:** run `clang-format` on any file you touch.
- **Lint:** `clang-tidy` is configured; fix warnings it raises.

If you don't have the tools installed, at least keep the style consistent with
the surrounding code (2-space-ish indent, `ssg::` prefix, `m_` for members).

---

## 📂 Where to add stuff

| You want to add... | Put it here |
| --- | --- |
| A new screen / state | `src/App/Scenes/` (inherit `IScene`) |
| A new logic slice | `src/App/Layers/` (inherit `ILayer`) |
| A new ECS component | `src/App/Components/` |
| A new input event | `src/App/Events/` |
| A core system (audio, etc.) | `src/Core/Systems/` |
| A rendering feature | `src/Core/Rendering/` |
| A shared type / alias | `src/Shared/Types.hpp` |
| An asset (texture, atlas) | `src/App/assets/` (copied next to the exe) |

---

## 🧱 Patterns to follow

### Add a new scene
1. Create `src/App/Scenes/MyScene.hpp/.cpp` inheriting `IScene`.
2. Implement `OnUpdate` / `OnRender` (forward to your layers).
3. In `Application::Run()`, `stack.Push(std::make_unique<MyScene>())`.

### Add a new layer
1. Create `src/App/Layers/MyLayer.hpp/.cpp` inheriting `ILayer`.
2. Implement `OnAttach`, `OnDetach`, `OnUpdate`, `OnRender`.
3. `scene->PushLayer(std::make_unique<MyLayer>())`.

### Add an ECS component
1. Add a plain struct in `src/App/Components/` (see `CTransform`).
2. `registry.emplace<CMyComp>(entity)` to attach it.
3. `registry.view<CMyComp>()` to read it in a system/layer.

### Listen to an event
```cpp
Engine::instance().eventBus.Sink<MyEvent>().connect<&MyClass::OnMyEvent>(this);
```
Queue events with `eventBus.Queue<MyEvent>(args...)`.

---

## ✅ Before you open a PR

- [ ] Project builds from a clean `build/` folder.
- [ ] `clang-format` applied.
- [ ] `clang-tidy` is clean (or warnings are understood).
- [ ] New code follows the scene/layer/ECS patterns above.
- [ ] Docs updated if you changed architecture or added a dependency.

---

## ❓ Not sure where something goes?

Read [`ARCHITECTURE.md`](ARCHITECTURE.md) for the flow, and
[`CLASSES.md`](CLASSES.md) for the per-class map. Then just ask. 🙂
