# 🌳 Transforms, Hierarchy & Systems

This document explains the transform pipeline, the parent/child hierarchy and the entity factories. Everything lives in the `ssg` namespace.

If you only remember one thing, remember this rule:

> **Write local, read world.**
> You edit `CTransform` (local). Everything visual/spatial reads
> `CWorldTransform` (world). Exactly one system converts between them, once per
> frame.

---

## 🧭 The core idea

A world position is **derived**, not stored. A child's world position is its
parent's world position combined with the child's own local offset. If we stored
world position and edited it by hand, it would constantly drift out of sync; the
classic source of hierarchy bugs.

So we split the concept into two components and let **one system** own the
conversion:

| Component | Meaning | Who writes it |
| --- | --- | --- |
| `CTransform` | **local** — relative to the parent (or the world, if a root) | you / gameplay |
| `CWorldTransform` | **world** — absolute, derived | **only** `TransformSystem` |

Roots (entities with no parent) have `world == local`.

---

## 🧩 The components

### `CTransform` (local)
```cpp
struct CTransform
{
    Vec2 position{0, 0};
    Vec2 scale{1, 1};    // a MULTIPLIER, not a pixel size
    float rotation{0};
};
```

> ⚠️ `scale` is a **multiplier** (default `{1,1}`), not a pixel size. Pixel size
> lives on `CSprite.size`. This is what makes hierarchical scaling correct: a
> `1×` child of a `1×` parent stays `1×`, it does not multiply into a giant.

### `CWorldTransform` (world, derived)
```cpp
struct CWorldTransform
{
    Vec2 position{0, 0};
    Vec2 scale{1, 1};
    float rotation{0};
};
```
Read-only to everyone except `TransformSystem`. Rendering, the spatial grid, and
the camera all read this.

### `CRelationship` (the hierarchy)
An **intrusive linked list**. A parent does not store an array of children — it
points at its *first* child, and each child points at its siblings.

```cpp
struct CRelationship
{
    std::size_t  children{0};       // number of direct children
    entt::entity first{entt::null}; // head of the child list
    entt::entity prev{entt::null};  // previous sibling
    entt::entity next{entt::null};  // next sibling
    entt::entity parent{entt::null};// up-link to the parent
};
```

It remains a linked list because:
- **No per-parent heap allocation** — the links live inside each entity's own component.
- **O(1) attach / detach** — just fix up neighbour pointers.
- **Cache-friendly** — all `CRelationship` components sit in one packed pool.

> A **root** is an entity with no `CRelationship`, or one whose `parent` is
> `entt::null`. You never "attach to null" — roots are simply the default.

---

## ⚙️ The transform pipeline

`TransformSystem` fills every `CWorldTransform` from `CTransform` + the hierarchy,
**parents before children**. It walks down from each root recursively, so
ordering is guaranteed without any sorting.

Composition (position-only projects can ignore scale/rotation):
```
world.scale    = parentWorld.scale * local.scale
world.rotation = parentWorld.rotation + local.rotation
world.position = parentWorld.position + parentWorld.scale * local.position
```

### Frame order (critical)
`TransformSystem::Update()` must run **before** anything that reads world
position.

> 🐛 If children appear to "lag one frame behind" their parent, this order is
> almost always wrong.

---

## 🔗 Attaching & detaching (`ssg::hierarchy`)

All linked-list surgery lives in two functions so call sites never touch
`first`/`prev`/`next` directly.

```cpp
void AttachChild(entt::registry&, entt::entity parent, entt::entity child);
void AttachChild(entt::registry&, entt::entity parent, entt::entity child, AttachMode mode);
void DetachChild(entt::registry&, entt::entity child);
```

### `AttachMode`
```cpp
enum class AttachMode : std::uint8_t
{
    KeepLocal = 0,      // default: child's local transform is left as-is
    KeepWorld = 1 << 0, // rebase local so the child's world position is unchanged
};
```

- **Plain `AttachChild`** is *pure hierarchy* — it knows nothing about
  transforms. Use it for any parent/child relationship (UI nodes, groupings,
  things with no transform at all).
- **The `AttachMode` overload** runs the plain attach, then applies the extra
  behaviour. `KeepWorld` rebases the child's local transform so it does not
  teleport when it gets a new parent. The rebase is guarded by `try_get`, so it
  safely no-ops if either entity lacks a transform.

### `KeepWorld` needs current world transforms
The rebase reads `CWorldTransform`. During the frame loop those are already
up to date. But during one-time setup (`OnAttach`), the loop has not run yet, so
call `TransformSystem::Update(0.f)` once before attaching:

```cpp
m_TransformSystem.Update(0.0f); // seed world transforms
hierarchy::AttachChild(registry, parent, child, hierarchy::AttachMode::KeepWorld);
```

### Turning a child back into a root / reparenting
| You want to... | Do this |
| --- | --- |
| Create a root | Just create the entity — don't attach it |
| Make B a child of A | `AttachChild(reg, A, B)` |
| Child back to a root | `DetachChild(reg, child)` |
| Reparent | `DetachChild(reg, child)` then `AttachChild(reg, newParent, child)` |

---

## 🏭 Entity factories (`ssg::factory`)

Factories **attach a standard set of components to an entity you already
created**. They do not create the entity — that keeps creation and component
setup separate.

```cpp
void AddDefaultTransform(entt::registry&, entt::entity);
void AddDefaultTexture(entt::registry&, entt::entity);
void AddDefaultSprite(entt::registry&, entt::entity, TextureID, const sf::FloatRect&,
                      Vec2 size = {100, 100});
```

| Factory | Adds |
| --- | --- |
| `AddDefaultTransform` | `CTransform` + `CWorldTransform` + `CRelationship` (root) |
| `AddDefaultTexture` | the above + an empty `CTexture` |
| `AddDefaultSprite` | the above + `CSprite` (origin `{0.5,0.5}`, `size`) + `CTexture` |

Factories guarantee the **local + world transform pair always
exists together**. A common hierarchy bug is an entity with a `CTransform` but no
`CWorldTransform`, which then gets silently skipped by the renderer's
`view<..., CWorldTransform>` or asserts on `get`. Going through a factory makes
that impossible.

Usage:
```cpp
auto e = registry.create();
factory::AddDefaultSprite(registry, e, texID, rect, {200, 200});
```

---

## ➡️ Related docs
- [`ARCHITECTURE.md`](ARCHITECTURE.md) — the big picture.
- [`CLASSES.md`](CLASSES.md) — per-class reference.
