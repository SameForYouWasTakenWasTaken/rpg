# 🧩 Scriptable Components

This document lists the ECS components Lua can add and edit, and explains how to expose a new one. For the surrounding API see [`SCRIPTING_API.md`](SCRIPTING_API.md). For pitfalls see [`SCRIPTING.md`](SCRIPTING.md).

If you only remember one thing, remember this rule:

> **A component is described once, in one field list.**
> The `ScriptComponent<C>` specialization names the component and lists its
> fields. That single list drives `add` (the init table), `get` (the proxy's
> properties) and everything else. Nothing else needs to change per component.

---

## 🧭 The core idea

Every scriptable component has a **string name** (`"transform"`, `"sprite"`, ...). Scripts use it with four entity methods:

```lua
e:add("sprite", { size = Vec2(64, 64) })   -- create (or replace) from a table
e:has("sprite")                            -- true / false
local s = e:get("sprite")                  -- live proxy, or nil
e:remove("sprite")
```

Field names are `snake_case` in Lua and `camelCase` in C++ (`zIndex` becomes `z_index`, `textureHandle` becomes `texture_handle`). The mapping is written by hand in the field list.

### How `add` builds a component
1. A default-constructed C++ component is created (`C c{}`), so **the defaults are the struct's default member values**.
2. Each field is copied from the init table if the key is present.
3. The result **replaces** any existing component of that type (`emplace_or_replace`).

Two things follow:

- Fields you leave out reset to the C++ default, even if the entity already had that component.
- Keys that don't match a field are ignored, so a typo does nothing.

### How `get` behaves
`get` returns a `ComponentRef` proxy (its Lua type is named `<name>Ref`, for example `transformRef`). Reading a field returns a copy and assigning a field writes through. See [gotcha 1](SCRIPTING.md#-gotchas).

---

## 📋 Registered components

| Name | C++ type | Purpose |
| --- | --- | --- |
| `"transform"` | `CTransform` | local position, rotation and scale |
| `"sprite"` | `CSprite` | how the entity is drawn (size, origin, flip, draw order) |
| `"texture"` | `CTexture` | which texture, and which region of it |

Everything else (`CWorldTransform`, `CRelationship`, `CHumanoid`, `CDefinition`, ...) is **not** exposed to Lua yet.

---

## 🔧 `"transform"`: `CTransform`

The **local** transform. See [`HIERARCHY.md`](HIERARCHY.md).

| Field | Lua type | Default | Notes |
| --- | --- | --- | --- |
| `position` | `Vec2` | `(0, 0)` | relative to the parent, or the world for a root |
| `rotation` | number | `0` | same unit as `CTransform::rotation` |
| `scale` | `Vec2` | `(1, 1)` | a **multiplier**, not a pixel size |

> ⚠️ `scale` is a multiplier. Pixel size lives on `sprite.size`.

```lua
e:add("transform", { position = Vec2(100, 100), rotation = 45 })

local t = e:get("transform")
t.scale = Vec2(2, 2)
```

---

## 🖼️ `"sprite"`: `CSprite`

| Field | Lua type | Notes |
| --- | --- | --- |
| `size` | `Vec2` | size in pixels, **before** scale is applied |
| `origin` | `Vec2` | the pivot as a fraction of the sprite. `(0.5, 0.5)` is the center |
| `z_index` | number | draw order |
| `flip_x` | boolean | mirror horizontally |
| `flip_y` | boolean | mirror vertically |
| `color` | not usable | no Lua color type exists yet. Leave it unset |

> 💡 The factories (`AddDefaultSprite`) set `origin` to `(0.5, 0.5)`. A sprite created from Lua starts from `CSprite`'s own defaults, so set `origin` explicitly if you rely on a centered pivot.

Defaults are whatever `CSprite` declares.

```lua
e:add("sprite", { size = Vec2(128, 128), origin = Vec2(0.5, 0.5), z_index = 2 })
```

---

## 🎨 `"texture"`: `CTexture`

| Field | Lua type | Default | Notes |
| --- | --- | --- | --- |
| `texture_handle` | `TextureHandle` | invalid handle | from `engine.assets.get_texture_handle()` |
| `float_rect` | `FloatRect` | `(0, 0, 0, 0)` | the region of the texture to draw, **in pixels** |

For a whole image, use the texture's full size. For an atlas, use the region's rect.

```lua
local handle  = engine.assets.get_texture_handle("assets/Textures/tree.png")
local texture = engine.assets.get_texture(handle)

e:add("texture", {
    texture_handle = handle,
    float_rect     = FloatRect(0, 0, texture.width, texture.height),
})
```

---

## 🧱 Adding a scriptable component

The example below exposes a `CHealth` component as `"health"`.

### 1. Make the component script-friendly
Use a plain struct with a **default value on every member**. Those defaults become the defaults Lua sees.

```cpp
// src/App/Components/CHealth.hpp
struct CHealth
{
    float current{100.0f};
    float max{100.0f};
    bool  invulnerable{false};
};
```

Only **public data members** can be exposed.

### 2. Make sure each field type is visible to Lua
Numbers, booleans and strings work as-is. `Vec2`, `FloatRect` and `TextureHandle` are already registered.

For any other type, register a usertype under `src/App/Scripting/Types/` and call its `register_*` function from `lua::api::lua_api_init` (`src/App/Scripting/MainAPI.cpp`).

### 3. Specialize `ScriptComponent<C>`
In `src/App/Scripting/ComponentView.hpp`:

```cpp
template <> struct ScriptComponent<CHealth>
{
    static constexpr std::string_view name = "health";
    static constexpr auto fields = std::make_tuple(
        Field{"current",      &CHealth::current},
        Field{"max",          &CHealth::max},
        Field{"invulnerable", &CHealth::invulnerable});
};
```

### 4. Add it to `ScriptComponents`
```cpp
using ScriptComponents = TypeList<CTransform, CSprite, CTexture, CHealth>;
```

Also include the component's header at the top of the file.

### 5. Done, then document it
`add`, `has`, `remove`, `get` and the proxy type are all generated from the list. Nothing else in C++ needs to change.

Add a section for the component to this file. If you keep LuaLS stubs for editor autocomplete, update those as well.

### Rules of thumb
- **Names are `snake_case`.** Component names and field names are part of the script-facing contract, so rename them deliberately.
- **Keep defaults meaningful.** A script that writes `e:add("health", {})` should get a sane component.
- **Write to local state, not derived state.** Don't expose data that another system owns and rewrites every frame (like `CWorldTransform`). Scripts would fight the system for it.

---

## ➡️ Related docs
- [`SCRIPTING.md`](SCRIPTING.md): concepts, lifecycle, gotchas.
- [`SCRIPTING_API.md`](SCRIPTING_API.md): every function and type.
- [`HIERARCHY.md`](HIERARCHY.md): local vs world transforms.
