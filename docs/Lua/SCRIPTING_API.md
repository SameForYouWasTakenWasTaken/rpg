# 📖 Lua API Reference

Every function, type and hook a script can use. For concepts and pitfalls read [`SCRIPTING.md`](SCRIPTING.md) first. For component fields see [`SCRIPTING_COMPONENTS.md`](SCRIPTING_COMPONENTS.md).

Conventions used below:

- `obj:method()` uses a **colon**. `engine.assets.get_texture()` and `Type.func()` use a **dot**.
- `→` marks the return value. A trailing `?` (`Vec2?`) means it can be `nil`.
- `Vec2`, `FloatRect` and `TextureHandle` are **values**. Every read gives you a copy.

---

## 🗺️ Overview

| Name | Kind | Notes |
| --- | --- | --- |
| [`_update(dt)`](#-hooks) | function you define | called once per frame |
| [`print(...)`](#-print) | global function | replaces Lua's `print` |
| [`Vec2`](#-vec2) | global type | 2D float vector |
| [`FloatRect`](#-floatrect) | global type | rectangle |
| [`engine.entity`](#-engineentity) | table | entity creation |
| [`Entity`](#-entity) | type | returned by `engine.entity.create()` |
| [`engine.assets`](#-engineassets) | table | textures |
| [`engine.assets.TextureHandle`](#texturehandle) | type | id of a loaded texture |
| [`engine.assets.Texture`](#texture) | type | read-only texture info |

---

## 🪝 Hooks

Functions **you** define as globals. The engine calls them.

### `_update(dt)`
```lua
function _update(dt)
    -- runs every frame
end
```

| Parameter | Type | Meaning |
| --- | --- | --- |
| `dt` | number | time since the last frame, in seconds |

Runs once per frame, **before** the scene updates (input, `TransformSystem`, spatial grid, combat, camera). See [the frame order](SCRIPTING.md#every-frame).

> ⚠️ Currently required: the engine calls it without checking that it exists.

---

## 🖨️ `print`

```lua
print(...)
```

Replaces Lua's `print`. Writes one **Info** entry to the logger with the category `lua`, so it appears in the in-game Console. The reported location is the calling script's file and line.

- Arguments are converted with `tostring` (so `__tostring` is respected) and joined with tabs.
- It returns nothing.

```lua
print("spawned", e:valid(), Vec2(1, 2))
-- [lua] spawned    true    Vec2(1.000000, 2.000000)
```

---

## 📐 `Vec2`

A 2D float vector (`glm::vec2`). Global.

### Constructors
| Call | Result |
| --- | --- |
| `Vec2()` | `(0, 0)` |
| `Vec2(s)` | `(s, s)` |
| `Vec2(x, y)` | `(x, y)` |
| `Vec2.new(...)` | same as calling `Vec2(...)` |

### Fields
| Field | Type | Access |
| --- | --- | --- |
| `x` | number | read / write |
| `y` | number | read / write |

### Methods
| Method | → | Description |
| --- | --- | --- |
| `v:length()` | number | length |
| `v:length_squared()` | number | squared length (no square root) |
| `v:normalized()` | `Vec2` | unit vector. Returns `(0, 0)` for a zero-length vector instead of NaN |
| `a:dot(b)` | number | dot product |
| `a:distance(b)` | number | distance between the two points |
| `a:lerp(b, t)` | `Vec2` | linear interpolation. `t` is **not** clamped |

### Operators
| Expression | → | Notes |
| --- | --- | --- |
| `a + b`, `a - b` | `Vec2` | |
| `v * s`, `s * v` | `Vec2` | scalar multiply |
| `a * b` | `Vec2` | **component-wise** |
| `v / s` | `Vec2` | scalar divide |
| `a / b` | `Vec2` | **component-wise** |
| `-v` | `Vec2` | |
| `a == b` | boolean | exact comparison |
| `tostring(v)` | string | `"Vec2(x, y)"`, six decimals |

```lua
local a = Vec2(3, 4)
print(a:length())                 -- 5.0
print(a:normalized())             -- Vec2(0.600000, 0.800000)
print(Vec2(0, 0):lerp(a, 0.5))    -- Vec2(1.500000, 2.000000)
```

---

## 🔲 `FloatRect`

A rectangle in `float` coordinates (`sf::FloatRect`). Global.

### Constructors
| Call | Result |
| --- | --- |
| `FloatRect()` | empty rect at the origin |
| `FloatRect(x, y, w, h)` | rect with top-left `(x, y)` and size `(w, h)` |

### Fields
| Field | Type | Access |
| --- | --- | --- |
| `x`, `y` | number | read / write (top-left corner) |
| `width`, `height` | number | read / write |

### Methods
| Method | → | Description |
| --- | --- | --- |
| `r:contains(px, py)` | boolean | is the point inside the rect |
| `a:intersects(b)` | boolean | do the rects overlap |
| `a:intersection(b)` | `FloatRect?` | the overlapping area, or `nil` if none |
| `r:center()` | number, number | **two** return values: `cx, cy` |

### Operators
| Expression | → |
| --- | --- |
| `a == b` | boolean |
| `tostring(r)` | `"FloatRect(x, y, w, h)"` |

```lua
local r = FloatRect(0, 0, 32, 32)
local cx, cy = r:center()                      -- 16, 16
local hit = r:intersection(FloatRect(16, 16, 32, 32))
if hit then print(hit) end                     -- FloatRect(16.0, 16.0, 16.0, 16.0)
```

---

## 🌐 `engine`

The root table. It holds the entity and asset modules.

### `engine.entity`

| Function | → | Description |
| --- | --- | --- |
| `engine.entity.create()` | `Entity` | creates an **empty** entity in the **current** scene's registry. It has no components |

The current scene is the top scene on the `SceneStack` when the call happens.

```lua
local e = engine.entity.create()
```

---

## 🧍 `Entity`

A handle to an ECS entity, bound to the registry it was created in. It cannot be constructed directly. Get one from `engine.entity.create()`.

An `Entity` is a handle only. **It is not garbage-collected.** Call `destroy()` when you're done with it.

### Methods
| Method | → | Description |
| --- | --- | --- |
| `e:valid()` | boolean | `true` while the entity exists |
| `e:destroy()` | | destroys the entity. `valid()` is `false` afterwards |
| `e:add(name, init?)` | | adds component `name`, filled from the optional `init` table. **Replaces** the component if already present |
| `e:has(name)` | boolean | does the entity have component `name` |
| `e:remove(name)` | | removes component `name`. Does nothing if it isn't there |
| `e:get(name)` | proxy? | a live proxy to the component, or `nil` if the entity doesn't have it |

`name` is one of `"transform"`, `"sprite"`, `"texture"`. Any other name raises `unknown component: <name>`. Fields and defaults are in [`SCRIPTING_COMPONENTS.md`](SCRIPTING_COMPONENTS.md).

### Operators
| Expression | → | Description |
| --- | --- | --- |
| `a == b` | boolean | same entity **and** same registry |

### `add` details
- `init` is a table of `field = value` pairs. Any field you omit takes its **default**, not its current value.
- Keys that aren't fields are ignored.
- `add` on an existing component resets it. Use `get` to change one field.

### `get` details
- The proxy is **live**: assigning `proxy.field = value` changes the real component.
- Reading a field returns a **copy**. See [gotcha 1](SCRIPTING.md#-gotchas).
- If the component (or entity) is removed later, using the proxy raises `component no longer exists`.

```lua
local e = engine.entity.create()
e:add("transform", { position = Vec2(10, 10) })

print(e:has("transform"))          -- true
local t = e:get("transform")
t.rotation = 45                    -- writes through
t.position = t.position + Vec2(5, 0)

e:remove("transform")
print(e:get("transform"))          -- nil

e:destroy()
print(e:valid())                   -- false
```

---

## 🖼️ `engine.assets`

Texture loading and inspection. Paths are relative to the working directory, for example `"assets/Textures/tree.png"`.

### Functions
| Function | → | Description |
| --- | --- | --- |
| `engine.assets.get_texture_handle(path)` | `TextureHandle` | loads the texture through the `AssetManager` and returns its handle |
| `engine.assets.get_texture(handle)` | `Texture` | read-only info about a loaded texture |

```lua
local handle  = engine.assets.get_texture_handle("assets/Textures/tree.png")
local texture = engine.assets.get_texture(handle)
print(texture.width, texture.height)
```

### `TextureHandle`
The strongly typed id of a loaded texture. Store it in the `texture` component's `texture_handle` field. It has no constructor. Get one from `get_texture_handle`.

The type itself lives at `engine.assets.TextureHandle`.

| Member | → | Description |
| --- | --- | --- |
| `h.id` | number | the raw id (read-only) |
| `h:valid()` | boolean | is this a real handle |
| `engine.assets.TextureHandle.get_invalid_value()` | | the "invalid" sentinel. Note the **dot**, it takes no `self` |

### `Texture`
Read-only info about a texture (`sf::Texture`). It has no constructor. The type lives at `engine.assets.Texture`.

| Member | Type | Description |
| --- | --- | --- |
| `t.width` | number | width in pixels |
| `t.height` | number | height in pixels |
| `t.smooth` | boolean | is smoothing (filtering) on |
| `t.repeated` | boolean | does it tile |
| `t.srgb` | boolean | is it sRGB |
| `t.native_handle` | number | the underlying graphics API handle |
| `t:size()` | `Vec2` | width and height as a vector (**a method**, unlike `width`/`height`) |

All properties are read-only.

---

## 🔗 Related docs
- [`SCRIPTING.md`](SCRIPTING.md): concepts, lifecycle, gotchas.
- [`SCRIPTING_COMPONENTS.md`](SCRIPTING_COMPONENTS.md): component fields and defaults.
