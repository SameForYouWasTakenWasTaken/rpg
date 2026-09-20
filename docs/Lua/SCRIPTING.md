# 🌙 Lua Scripting

This document explains how scripting fits into the engine: where scripts live, when they run, what they can touch, and the pitfalls to avoid. Scripting uses **Lua 5.4** bound through **sol2**. The C++ side lives in the `ssg` namespace (`ssg::lua` for the script host, `ssg::lua::api` for the bindings).

For the full list of functions and types see [`SCRIPTING_API.md`](SCRIPTING_API.md). For the components a script can add and edit see [`SCRIPTING_COMPONENTS.md`](SCRIPTING_COMPONENTS.md).

If you only remember one thing, remember this rule:

> **Lua holds handles. C++ owns the data.**
> An `Entity` in Lua is a handle to an ECS entity. `entity:get("transform")`
> returns a live *proxy* to a component, not the component itself. Reading a
> field gives you a **copy**; assigning a field writes through.

---

## 🧭 The core idea

Entities, components and assets all live in C++. Lua never owns them. It only holds small objects that point at them, and every call goes back through a binding.

| In Lua | In C++ | Behaves like |
| --- | --- | --- |
| `Entity` | `lua::api::entity::Entity` (`entt::entity` + `entt::registry&`) | a handle; **not** garbage-collected |
| component proxy (`e:get("transform")`) | `ComponentRef<C>` (wraps an `entt::handle`) | a live reference; throws if the component is gone |
| `Vec2` | `glm::vec2` | a value; copied on every read |
| `FloatRect` | `sf::FloatRect` | a value |
| `TextureHandle` | `TextureHandle` | a value |
| `Texture` | `sf::Texture` | read-only info about a loaded texture |

Three consequences follow, and most scripting bugs come from forgetting one of them:

1. **Dropping a Lua variable does not destroy the entity.** Call `entity:destroy()`.
2. **Reads copy.** `t.position.x = 5` changes a temporary, not the transform.
3. **Proxies can go stale.** If the entity or component is removed, using an old proxy raises an error.

---

## 📁 Where things live

```
src/Core/Scripting/     ScriptEngine: owns the sol::state, runs files
src/App/Scripting/      The game-facing API: bindings for entities, components, assets, types
src/App/scripts/        The Lua scripts themselves
```

> `Scripting/` (capital S, C++) is the **bindings**. `scripts/` (lowercase, Lua) is the **content**. Don't mix them up.

`ScriptEngine` is owned by `Engine` (`engine.GetScriptEngine()`), not by a scene. The Lua state outlives scenes, while entities and registries belong to the scene.

### How scripts reach the executable
`src/App/scripts/` is copied next to the executable as `scripts/` by a post-build step (`clone_assets` in `src/CMakeLists.txt`). Paths given to `runFile` are relative to the working directory, so `scripts/foo.lua` resolves against the build output.

> 💡 The copy runs after `Game2` is built. If an edit to a script doesn't show up, rebuild, or copy the file into the build output by hand.

---

## ⚙️ Lifecycle

### Startup
In `Application::Run`, in this order:

1. `Engine` constructs its `ScriptEngine`, which opens the `base`, `math`, `string` and `table` libraries.
2. `lua::api::lua_api_init(state, engineContext, sceneStack)` registers every function and type. It **must** run before any scene or layer is created.
3. The scene is created and pushed onto the `SceneStack`, then its layers are attached.
4. `GameLayer::OnAttach` runs `scripts/other_test.lua` (currently hard-coded).

A script's top-level code runs **once**, when the file is loaded. Use `_update` for anything that should happen every frame.

### Every frame
```
renderer.Begin()
    _update(dt)              <- Lua
    SceneStack::Update(dt)   <- layers: input, TransformSystem, SpatialGrid, CombatSystem, camera
    SceneStack::Render()
renderer.End()
```

`_update` runs **before** the scene updates. Anything a script changes is seen by `TransformSystem` the same frame.

Scripts follow the same rule as gameplay code, described in [`HIERARCHY.md`](HIERARCHY.md): **write local, read world.** The `transform` component is the *local* transform. `TransformSystem` derives the world transform from it.

---

## 🚀 Your first script

```lua
-- scripts/my_script.lua
local e = engine.entity.create()

local handle  = engine.assets.get_texture_handle("assets/Textures/tree.png")
local texture = engine.assets.get_texture(handle)

e:add("transform", { position = Vec2(100, 100), scale = Vec2(1, 1) })
e:add("texture",   { texture_handle = handle,
                     float_rect = FloatRect(0, 0, texture.width, texture.height) })
e:add("sprite",    { size = Vec2(128, 128) })

function _update(dt)
    local t = e:get("transform")
    t.position = t.position + Vec2(20, 0) * dt   -- assign a whole Vec2
end
```

Load it from C++ for now:

```cpp
engine.GetScriptEngine().runFile("scripts/my_script.lua");
```

---

## 🔒 What a script can use

| Available | Not available |
| --- | --- |
| `base` (`pcall`, `error`, `assert`, `type`, `tostring`, `pairs`, `ipairs`, `select`, `setmetatable`, ...) | `io`, `os` |
| `math` | `package` / `require` |
| `string` | `coroutine`, `utf8`, `debug` |
| `table` | |

> ⚠️ This is a trimmed environment, **not a security sandbox**. `base` still contains `dofile`, `loadfile` and `load`, so a script can read and run other files from disk. Don't treat scripts as untrusted code yet.

There is no `require`. Until a loader exists, `dofile("scripts/other.lua")` is the only way to split code across files.

---

## 🪵 Output and errors

### `print`
`print` is replaced by an engine version. It writes to the logger at **Info** level with the category `lua`, so it shows up in the in-game Console. The source location is the **script's** file and line, not a C++ line.

- Arguments are joined with tabs, like standard Lua.
- Each argument is converted with `tostring`, so `__tostring` is respected (`Vec2` prints as `Vec2(100.000000, 100.000000)`).

### Errors from the engine
Errors raised inside a binding become ordinary Lua errors. Examples:

| Message | Cause |
| --- | --- |
| `unknown component: foo` | `add` / `has` / `remove` / `get` with a name that isn't registered |
| `component no longer exists` | a component proxy was used after the component or entity was removed |

Wrap risky calls in `pcall` if the script should keep going.

### Script load errors
`ScriptEngine::runFile` returns `false` on failure and prints `[lua] load error: ...` or `[lua] runtime error: ...` to **stderr** (the terminal), not to the Console.

---

## ⚠️ Gotchas

**1. Reads copy, so nested writes do nothing.**
```lua
local t = e:get("transform")
t.position.x = 5                       -- ❌ changes a temporary
t.position = Vec2(5, t.position.y)     -- ✅ assign the whole value
```

**2. `add` replaces.**
Calling `add` on a component the entity already has **replaces** it, and any field you leave out goes back to its default. To change a field, use `get` and assign to it.

**3. Unknown keys are ignored.**
`e:add("transform", { postion = Vec2(1, 1) })` silently does nothing. Double-check field names against [`SCRIPTING_COMPONENTS.md`](SCRIPTING_COMPONENTS.md).

**4. Entities are not garbage-collected.**
An entity lives until you call `destroy()` (or C++ destroys it). Losing the Lua reference leaks it.

**5. After `destroy()`, only `valid()` is safe.**
Check `e:valid()` before using an entity that C++ or another script may have destroyed.

**6. Methods use a colon.**
`e:valid()`, `v:length()`. A dot (`e.valid()`) passes no `self` and fails. Free functions such as `engine.entity.create()` use a dot.

**7. Don't keep entities across scenes.**
An `Entity` holds a reference to the registry it was created in. When the scene is destroyed, that reference dangles.

**8. Rendering needs a world transform.**
`add("transform")` adds only the local `CTransform`. The renderer draws entities that have `CSprite`, `CTexture` **and** `CWorldTransform`, and `CWorldTransform` is owned by `TransformSystem`, so Lua cannot add it. If a scripted entity doesn't appear, check that a world transform is being created for it.

---

## 🚧 Known limitations

These describe the current state. Delete each line as it gets fixed.

- **`_update` is required.** It is called every frame without a nil check, so a script set that doesn't define it will fail. Define it, even if empty.
- **`_update` errors aren't caught.** A Lua error inside `_update` is not handled by the engine.
- **Scripts are loaded from C++.** `GameLayer::OnAttach` hard-codes `scripts/other_test.lua`. There is no script loader or per-scene script list yet.
- **Script errors skip the Console.** See [Script load errors](#script-load-errors).
- **`sprite.color` isn't usable.** There is no Lua color type yet.
- **Only three components are exposed:** `transform`, `sprite`, `texture`.

---

## ➡️ Related docs
- [`SCRIPTING_API.md`](SCRIPTING_API.md): every function, type and hook.
- [`SCRIPTING_COMPONENTS.md`](SCRIPTING_COMPONENTS.md): components, and how to expose a new one.
- [`HIERARCHY.md`](HIERARCHY.md): local vs world transforms.
- [`ARCHITECTURE.md`](ARCHITECTURE.md): the big picture.
