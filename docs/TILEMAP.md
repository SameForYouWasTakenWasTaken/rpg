# 🗺️ Tilemap System

This document explains the tilemap implementation used by the RPG project, how Tiled maps are imported, how tilesets are represented at runtime, and how a map becomes renderable ECS entities.

The current implementation is intentionally small: it provides the data-loading and rendering foundation for Tiled maps, while leaving more advanced map features for later work.

---

## 🔄 Overview

The tilemap pipeline is:

```text
                         Tiled
                           │
             ┌─────────────┴─────────────┐
             │                           │
          map.tmj                    tileset.tsx
             │                           │
             │                     tileset metadata
             │                           │
             └──────────────┬────────────┘
                            │
                            ▼
                  Tiled::LoadTilemapJSON()
                            │
                 ┌──────────┼──────────┐
                 ▼          ▼          ▼
             TileLayer   Tileset   ObjectLayer
                 │          │          │
                 │          │       (future)
                 │          │
                 └──────┬───┘
                        ▼
                    Tilemap
                        │
                        ▼
                 GameLayer::OnAttach()
                        │
              one ECS entity per tile
                        │
             ┌──────────┼──────────┐
             ▼          ▼          ▼
          Transform   Texture    Sprite
             │          │          │
             └──────────┴──────────┘
                        │
                        ▼
                    Renderer
```

The important distinction is that **`Tilemap` is map data, not a renderer**. It stores the information necessary to interpret the map. `GameLayer` currently turns that information into ECS entities, and the existing renderer handles those entities like any other sprite.

---

## 🛠️ Map Creation Tools

The project currently uses two external tools as part of the asset workflow.

### Tiled

[Tiled](https://www.mapeditor.org/) is the map editor used to create and edit the game's tilemaps.

Its documentation is available at:

- [Tiled Documentation](https://doc.mapeditor.org/en/latest/)
- [Working with Layers](https://doc.mapeditor.org/en/latest/manual/layers/)
- [Tiled Projects](https://doc.mapeditor.org/en/latest/manual/projects/)

Tiled is responsible for the **map itself**: placing tiles, organizing tile layers, configuring map dimensions, and referencing external tilesets.

The engine currently consumes Tiled's JSON map format (`.tmj`). The map should be a finite, orthogonal map using normal tile layers for the functionality currently implemented.

Tiled supports many more features than the engine currently consumes. For example, Tiled supports object layers, image layers, group layers, infinite maps, custom properties, and other map features. These can be introduced into the engine later without changing the basic purpose of the `Tilemap` class.

### TexturePacker

[TexturePacker](https://www.codeandweb.com/texturepacker) is used to build the texture atlases used by the game.

Official documentation:

- [TexturePacker Documentation](https://www.codeandweb.com/texturepacker/documentation)
- [TexturePacker Support / FAQ](https://www.codeandweb.com/texturepacker/support)

TexturePacker takes individual images/sprites and packs them into atlas textures. The resulting atlas can then be referenced by the project's rendering/asset systems.

For tilemaps, the important part is that the image referenced by the Tiled tileset corresponds to the atlas containing the tiles. The `.tsx` file describes the dimensions and layout of the tiles in that image; the actual image is loaded by the engine's `AssetManager`.

---

## 📁 Recommended Asset Layout

A map and its external tileset should live together. This keeps the Tiled asset structure easy to move and maintain when importing a map or receiving a complete map/tileset package.

For example:

```text
assets/
└── data/
    └── maps/
        └── forest/
            ├── forest.tmj
            └── forest.tsx
```

The map references the external tileset through the `source` field:

```json
{
    "firstgid": 1,
    "source": "forest.tsx"
}
```

The tileset then describes the atlas image and tile layout:

```xml
<tileset
    version="1.10"
    name="forest"
    tilewidth="32"
    tileheight="32"
    tilecount="1980"
    columns="44">
    <image source="assets/Textures/Atlas/forest.png" />
</tileset>
```

Keeping the `.tmj` and `.tsx` together means the map's external tileset reference remains straightforward when moving an entire map directory between projects or asset packages.

> **Current implementation note:** the `.tsx` file is resolved from the map through its `source` field, while the image path inside the `.tsx` is passed to the `AssetManager`. The image path therefore needs to resolve correctly through the project's asset-loading setup.

---

## 🧱 Runtime Data Model

The tilemap implementation is split into three main pieces:

- `Tilemap`
- `TileLayer`
- `Tileset`

There is also an `ObjectLayer` type prepared for future support, but object-layer loading is not implemented yet.

### `Tilemap`

`Tilemap` is the container for an entire loaded map.

It stores:

- map width in tiles
- map height in tiles
- tile width in pixels
- tile height in pixels
- all tile layers
- all tilesets
- object layers

The public API currently provides:

```cpp
std::uint32_t getWidth() const;
std::uint32_t getHeight() const;
std::uint32_t getTileWidth() const;
std::uint32_t getTileHeight() const;

const Vector<TileLayer>& getTileLayers() const;
const Vector<Tileset>& getTilesets() const;
const Vector<ObjectLayer>& getObjectLayers() const;

const Tileset* getTilesetForGid(TileLayer::TileID gid) const;
```

The class itself does not create entities or submit anything to the renderer.

### `TileLayer`

A `TileLayer` represents one Tiled tile layer.

It contains:

- the layer name
- layer width
- layer height
- a flat array of tile GIDs

A tile can be accessed with:

```cpp
const auto gid = layer.at(x, y);
```

Internally, the two-dimensional coordinate is converted into a row-major array index:

```text
index = y * width + x
```

So the underlying data is equivalent to:

```text
row 0: [0, 1, 2, 3, ...]
row 1: [0, 1, 2, 3, ...]
row 2: [0, 1, 2, 3, ...]
...
```

A GID of `0` represents an empty tile and is skipped by the current rendering code.

### `Tileset`

A `Tileset` describes the texture and layout needed to turn a Tiled global tile ID into a texture region.

It stores:

- tileset name
- tile width
- tile height
- number of columns
- tile count
- `firstgid`
- the engine's `TextureID`

The important field for map interpretation is `firstgid`.

Tiled assigns global IDs to tilesets. For example:

```text
Tileset A: firstgid = 1
Tileset B: firstgid = 1981
Tileset C: firstgid = 3961
```

When a map contains a GID, the engine finds the tileset with the highest `firstgid` that is still less than or equal to that GID.

This is what `Tilemap::getTilesetForGid()` does. It searches the tilesets from the back, which is appropriate because later tilesets have higher starting GIDs.

Once the correct tileset is found, the global GID becomes a tileset-local ID:

```cpp
const auto localId = gid - tileset->getFirstGid();
```

---

## 🎨 Converting a Tile ID to an Atlas Region

The local tile ID is converted into an atlas rectangle by `Tileset::getRegion()`.

Given:

```text
columns = 44
tileWidth = 32
tileHeight = 32
localId = 45
```

the tile's position in the atlas is:

```text
column = localId % columns
row    = localId / columns
```

The resulting texture rectangle is:

```text
x = column * tileWidth
y = row    * tileHeight
w = tileWidth
h = tileHeight
```

For the example above:

```text
column = 45 % 44 = 1
row    = 45 / 44 = 1

region = (32, 32, 32, 32)
```

This is why the `.tsx` metadata must accurately describe the atlas it references. The engine does not need a separate region for every tile; it calculates the region from the tile ID and the tileset's dimensions.

---

## 📥 Loading a Tiled Map

Maps are loaded with:

```cpp
map::Tilemap tilemap =
    map::Tiled::LoadTilemapJSON(
        m_EngineContext,
        "data/maps/random/random_map.tmj"
    );
```

`LoadTilemapJSON()` performs the following steps:

### 1. Open the `.tmj`

The loader opens the map file as JSON and validates the presence of the main map properties:

- `width`
- `height`
- `tilewidth`
- `tileheight`
- `layers`
- `tilesets`

Missing required properties result in an exception.

### 2. Read map dimensions

The map dimensions and tile dimensions become the corresponding `Tilemap` properties.

For example, the current test map is 30 × 20 tiles with 32 × 32 pixel tiles.

### 3. Load tile layers

Each entry in `layers` is inspected by its Tiled `type`.

For `tilelayer`, the loader reads:

- `name`
- `width`
- `height`
- `data`

and creates a `TileLayer`.

### 4. Load external tilesets

Each map tileset entry must currently provide:

```json
{
    "firstgid": 1,
    "source": "test.tsx"
}
```

The `source` is resolved relative to the map's directory and passed to `LoadTilesetXML()`.

### 5. Parse the `.tsx`

The external tileset is XML, parsed with `tinyxml2`.

The loader reads:

- `name`
- `tilewidth`
- `tileheight`
- `tilecount`
- `columns`
- the `<image source="...">` path

The referenced image is then loaded through the engine's `AssetManager`.

The result is a `Tileset` containing both the metadata and the resulting `TextureID`.

### 6. Return the `Tilemap`

The loaded layers and tilesets are placed into a single `Tilemap` object.

The map loader currently creates an empty object-layer collection. Object-group parsing is deliberately left for future work.

---

## 🖼️ Rendering a Tilemap

The current renderer does not have a specialized `TilemapRenderer`.

Instead, `GameLayer` converts every non-empty tile into a normal ECS entity.

The process is:

```text
TileLayer
   │
   │ gid
   ▼
find Tileset
   │
   │ local ID
   ▼
calculate atlas Region
   │
   ▼
create ECS entity
   │
   ├── CTransform
   ├── CSprite
   └── CTexture
```

The tile's world position is calculated from its grid coordinate:

```cpp
transform.position = {
    static_cast<float>(x * tilemap.getTileWidth()),
    static_cast<float>(y * tilemap.getTileHeight())
};
```

The entity then receives:

- the tileset's `TextureID`
- the calculated atlas rectangle
- a sprite size equal to the map's tile dimensions
- a z-index corresponding to the tile layer's order

The current `GameLayer` increments the z-index after every tile layer, meaning later tile layers render above earlier ones.

The normal renderer then handles the tile just like any other sprite entity.

---

## 💡 Basic Usage Example

A minimal consumer currently looks like this:

```cpp
map::Tilemap tilemap =
    map::Tiled::LoadTilemapJSON(
        m_EngineContext,
        "data/maps/forest/forest.tmj"
    );

zIndex_t zIndex = 0;

for (const auto& layer : tilemap.getTileLayers())
{
    for (std::uint32_t y = 0; y < layer.getHeight(); ++y)
    {
        for (std::uint32_t x = 0; x < layer.getWidth(); ++x)
        {
            const auto gid = layer.at(x, y);

            if (gid == 0)
                continue;

            const auto* tileset = tilemap.getTilesetForGid(gid);
            if (!tileset)
                continue;

            const auto localId = gid - tileset->getFirstGid();
            const auto region = tileset->getRegion(localId);

            // Create a normal CTexture / CSprite entity here.
        }
    }

    ++zIndex;
}
```

The actual implementation in `GameLayer` wraps the entity creation into a `makeTile()` helper.

---

## 📦 Current File Formats

### `.tmj`

The map format currently consumed by the loader.

It is Tiled's JSON map representation and contains the map dimensions, layers, and references to tilesets.

### `.tsx`

The external tileset format currently consumed by the loader.

It contains tileset metadata and the image used as the tileset texture.

### Atlas image

The image referenced by the `.tsx` is the actual texture containing the tiles. The project uses TexturePacker to produce atlas textures, but the tilemap loader itself does not parse TexturePacker's JSON atlas metadata. It only needs the image and the tile-grid metadata from the `.tsx`.

---

## ⚠️ Current Limitations

This implementation is the first foundation rather than a complete Tiled feature set.

Currently supported:

- finite orthogonal Tiled maps
- external `.tsx` tilesets
- multiple tile layers
- multiple tilesets
- global tile IDs (`gid` / `firstgid`) and tileset lookup
- atlas region calculation
- conversion of tiles into normal ECS sprite entities
- layer ordering through z-index

Not currently implemented:

- Tiled object-layer loading
- object properties and object metadata
- image layers
- group layers
- infinite maps/chunked tile data
- tile flipping/rotation flags
- tile animations
- Tiled custom properties
- terrain/automapping metadata
- a dedicated tilemap renderer
- culling of off-screen tiles

These limitations are important when creating maps: **the current engine should only be given maps using the subset of Tiled functionality that the loader understands.**

---

## 🧭 Design Direction

The implementation deliberately keeps Tiled-specific parsing separate from the runtime map representation.

```text
Tiled files
    │
    ▼
Tiled::LoadTilemapJSON
    │
    ▼
Runtime Tilemap / TileLayer / Tileset
    │
    ▼
Game systems
```

This means gameplay code does not need to parse JSON or XML itself. It receives a structured `Tilemap` and can work with tile layers and tilesets directly.

It also leaves room for the map loader to grow. More Tiled features can be added to the loader and represented in the runtime types without forcing the rest of the engine to understand the `.tmj` or `.tsx` file formats directly.

---

## 📚 Relevant Source Files

The tilemap implementation currently lives primarily in:

```text
src/App/
├── Factories/Gameplay/Map/
│   ├── MapLoader.cpp
│   └── MapLoader.hpp
│
├── Map/
│   ├── MapObject.hpp
│   ├── ObjectLayer.hpp
│   ├── TileLayer.cpp
│   ├── TileLayer.hpp
│   ├── Tilemap.cpp
│   ├── Tilemap.hpp
│   ├── Tileset.cpp
│   └── Tileset.hpp
│
├── Layers/
│   └── GameLayer.cpp       # currently turns map tiles into ECS entities
│
└── data/maps/
    └── random/
        ├── random_map.tmj
        └── test.tsx
```

For the complete implementation history, see the [Tilemap feature PR](https://github.com/SameForYouWasTakenWasTaken/rpg/pull/63).
