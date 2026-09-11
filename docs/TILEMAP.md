# 🗺️ Tilemap System

This document describes the tilemap implementation used by the RPG project: how maps are authored in Tiled, how external tilesets are loaded, how tile IDs are resolved, and how the resulting map data can be consumed by ECS code.

The implementation is intentionally focused on the data-loading foundation. Advanced Tiled features are not part of the current runtime contract.

---

## 🔄 Overview

The current asset flow is:

```text
Tiled map (.tmj)
       │
       ├── tile layers
       │
       └── external tilesets
                │
                ▼
       Tileset (.tsj) + image
                │
                ▼
       Tiled::LoadTilemapJSON()
                │
                ▼
             Tilemap
          ┌─────┴─────┐
          ▼           ▼
      TileLayer    Tileset
          │           │
          └─────┬─────┘
                ▼
       optional ECS conversion
                │
                ▼
      O_N2::CreateEntitiesForMap()
```

`Tilemap` is a data representation of a loaded map. It does not own rendering logic or create ECS entities.

---

## 🛠️ Map Creation Tools

The project currently uses **Tiled** for map creation and **TexturePacker** for atlas generation.

### Tiled

[Tiled](https://www.mapeditor.org/) is the map editor used to create the project's tilemaps.

Official documentation:

- [Tiled Documentation](https://doc.mapeditor.org/en/latest/)
- [Working with Layers](https://doc.mapeditor.org/en/latest/manual/layers/)
- [Tiled Projects](https://doc.mapeditor.org/en/latest/manual/projects/)

The engine currently consumes Tiled's JSON map format (`.tmj`). The supported map configuration is a finite, orthogonal map containing normal tile layers and references to external JSON tilesets (`.tsj`).

Tiled supports many additional features that are not currently consumed by the loader, including object layers, image layers, group layers, infinite maps, custom properties, tile animations, and tile transformations.

### TexturePacker

[TexturePacker](https://www.codeandweb.com/texturepacker) is used to create texture atlases.

Official documentation:

- [TexturePacker Documentation](https://www.codeandweb.com/texturepacker/documentation)
- [TexturePacker Support](https://www.codeandweb.com/texturepacker/support)

The tilemap loader does not parse TexturePacker's JSON metadata. It loads the image referenced by the tileset and calculates tile regions from the tileset's grid metadata.

---

## 📁 Asset Layout

External tilesets should be kept alongside the map that references them. This is especially useful when maps and their assets are moved as a package.

For example:

```text
assets/
└── data/
    └── maps/
        └── forest/
            ├── forest.tmj
            └── forest.tsj
```

The map references the tileset through a `source` entry:

```json
{
    "firstgid": 1,
    "source": "forest.tsj"
}
```

The external tileset contains its metadata and image reference as JSON:

```json
{
    "columns": 44,
    "image": "assets/Textures/Atlas/forest.png",
    "imageheight": 1441,
    "imagewidth": 1413,
    "margin": 0,
    "name": "forest",
    "spacing": 0,
    "tilecount": 1980,
    "tiledversion": "1.12.1",
    "tileheight": 32,
    "tilewidth": 32,
    "type": "tileset",
    "version": "1.10"
}
```

### Path handling

The `.tsj` path is resolved relative to the directory containing the `.tmj` file.

The image path inside the `.tsj` is then passed to `AssetManager::LoadTexture()`. In this project, asset paths ultimately follow the executable-relative asset layout used by the application. The tileset JSON paths are deliberately written to remain compatible with the project's directory/indexing layout.

Keeping the map and external tileset together avoids unnecessary complexity when importing a complete map package. The `source` field is the part that must remain correct when the map package is moved.

---

## 🧱 Runtime Data Model

The runtime representation consists of three primary types:

- `Tilemap`
- `TileLayer`
- `Tileset`

An `ObjectLayer` type also exists, but object-group loading is not implemented yet.

### `Tilemap`

`Tilemap` stores the data required to interpret an entire map.

It contains:

- map width in tiles
- map height in tiles
- tile width in pixels
- tile height in pixels
- tile layers
- tilesets
- object layers

Its current public API includes:

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

### `TileLayer`

A `TileLayer` represents one Tiled tile layer.

It stores:

- layer name
- layer width
- layer height
- a flat vector of tile GIDs

The tile ID at a coordinate can be retrieved with:

```cpp
const auto gid = layer.at(x, y);
```

The underlying data is row-major:

```text
index = y * width + x
```

A GID of `0` represents an empty tile and does not produce a tile entity in the current ECS conversion helper.

### `Tileset`

A `Tileset` contains the information required to resolve a global Tiled tile ID into a texture and texture region.

It stores:

- tileset name
- tile width
- tile height
- number of columns
- tile count
- `firstgid`
- the engine's `TextureHandle`

The current public API includes:

```cpp
std::uint32_t getColumns() const;
String getName() const;
std::uint32_t getWidth() const;
std::uint32_t getHeight() const;
std::uint32_t getTileCount() const;
std::uint32_t getFirstGid() const;
TextureHandle getTextureID() const;

Region getRegion(std::uint32_t localId) const;
```

---

## 🔢 Global Tile IDs and `firstgid`

Tiled stores tile IDs in map layers as **global IDs (GIDs)**. A GID is not directly an index into one particular tileset.

Each tileset has a `firstgid` assigned by the map.

For example:

```text
Tileset A: firstgid = 1
Tileset B: firstgid = 1981
Tileset C: firstgid = 3961
```

Given a GID, the loader finds the tileset whose `firstgid` is the greatest value that is still less than or equal to the GID.

After selecting the tileset, the GID is converted into a tileset-local ID:

```cpp
const auto localId = gid - tileset->getFirstGid();
```

For example, with `firstgid = 1981`:

```text
gid     = 2026
localId = 2026 - 1981
        = 45
```

This local ID is what the tileset uses to calculate the corresponding texture region.

---

## 🎨 Tile Region Calculation

`Tileset::getRegion()` converts a tileset-local tile ID into a texture rectangle.

Given:

```text
columns    = 44
tileWidth  = 32
tileHeight = 32
localId    = 45
```

the grid coordinate is calculated as:

```text
column = localId % columns
row    = localId / columns
```

Therefore:

```text
column = 45 % 44 = 1
row    = 45 / 44 = 1
```

The resulting region is:

```text
x = column * tileWidth
  = 1 * 32
  = 32

y = row * tileHeight
  = 1 * 32
  = 32

width  = 32
height = 32
```

So the final rectangle is:

```text
(32, 32, 32, 32)
```

This calculation assumes the image is laid out as a regular grid matching the tileset's `tilewidth`, `tileheight`, and `columns` values.

---

## 📥 Loading a Map

Maps are loaded through:

```cpp
map::Tilemap tilemap =
    map::Tiled::LoadTilemapJSON(
        m_EngineContext,
        "data/maps/random/random_map.tmj"
    );
```

The loader performs the following operations.

### 1. Open and parse the `.tmj`

The file is opened as JSON.

The following top-level properties are required:

- `height`
- `width`
- `tileheight`
- `tilewidth`
- `layers`
- `tilesets`

Missing required properties cause an exception.

### 2. Read map dimensions

The map's width, height, tile width, and tile height are stored in the resulting `Tilemap`.

### 3. Load tile layers

Each entry in `layers` is inspected using its Tiled `type`.

For `tilelayer`, the loader reads:

- `name`
- `width`
- `height`
- `data`
- `id`

The `data` array becomes the layer's flat vector of GIDs.

Object groups and other layer types are currently ignored.

### 4. Resolve external tilesets

Each entry in `tilesets` must provide:

```json
{
    "firstgid": 1,
    "source": "test.tsj"
}
```

The `source` is combined with the map's parent directory and passed to the JSON tileset loader.

### 5. Parse the `.tsj`

The external tileset is parsed as JSON.

The loader validates and reads these fields:

- `name`
- `tilewidth`
- `tileheight`
- `tilecount`
- `columns`
- `type`
- `image`

The `type` must be `"tileset"`.

The `image` path is passed to the engine's `AssetManager`, which loads/reuses the texture and returns a `TextureHandle`.

### 6. Return the runtime representation

The loader returns one `Tilemap` containing all successfully loaded tile layers and tilesets.

The object-layer collection is currently empty because object-group parsing has not been implemented.

---

## 🧩 Converting Tilemaps to ECS Entities

Tilemap loading and entity creation are deliberately separate responsibilities.

The current straightforward conversion helper is:

```cpp
map::O_N2::CreateEntitiesForMap(registry, tilemap);
```

For every non-empty GID, it:

1. Finds the corresponding tileset.
2. Converts the GID to a tileset-local ID.
3. Calculates the texture region.
4. Creates an EnTT entity.
5. Adds `CTransform`, `CSprite`, and `CTexture`.
6. Stores the tileset's `TextureHandle` and calculated texture region.
7. Positions the entity using the tile's grid coordinates.
8. Uses the tile layer's order as the sprite z-index.

The implementation is intentionally placed in `ssg::map::O_N2`. It performs a nested traversal over every tile in every layer and does not use spatial chunking or partitioning. The namespace makes that O(n²) characteristic explicit rather than hiding it behind a generic function name.

This function is an ECS conversion helper, not a dedicated tilemap renderer. The `Tilemap` classes remain independent of EnTT entity creation.

---

## 🖼️ Consuming Tilemap Data Directly

A consumer can still iterate through the layers and resolve each non-empty GID independently:

```cpp
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

            // Use tileset->getTextureID(), region, and the grid position.
        }
    }
}
```

The tile's position in map space is derived from its grid coordinate:

```cpp
Vec2 position{
    static_cast<float>(x * tilemap.getTileWidth()),
    static_cast<float>(y * tilemap.getTileHeight())
};
```

Layer order can be mapped to the renderer's z-index convention by the consuming system.

---

## 📦 Supported File Formats

### `.tmj`

The JSON map format produced by Tiled.

The current loader consumes the map dimensions, tile layers, and external `.tsj` tileset references.

### `.tsj`

The JSON external tileset format produced by Tiled.

The current loader consumes the tileset's basic grid metadata, validates that the JSON represents a tileset, and loads its image reference.

### Atlas image

The image referenced by the external tileset is loaded as the texture used for the tiles.

The current loader does not consume TexturePacker JSON metadata; the tileset's grid information is sufficient for region calculation.

---

## ⚠️ Current Limitations

Currently supported:

- finite orthogonal maps
- external `.tsj` tilesets
- multiple tile layers
- multiple tilesets
- global GID / `firstgid` resolution
- regular grid-based texture-region calculation
- loading tileset textures through `AssetManager`
- straightforward O(n²) tile-to-ECS conversion through `O_N2::CreateEntitiesForMap()`

Not currently implemented:

- object-group parsing
- object properties
- image layers
- group layers
- infinite maps / chunked data
- tile flipping and rotation flags
- tile animations
- custom Tiled properties
- terrain and automapping metadata
- dedicated tilemap rendering
- off-screen tile culling
- spatially chunked tile entity creation

Maps should therefore stay within the supported subset of Tiled until the corresponding loader functionality is implemented.

---

## 🧭 Design Direction

The tilemap implementation keeps file-format parsing separate from the runtime map representation and ECS conversion:

```text
Tiled files
    │
    ▼
MapLoader
    │
    ▼
Tilemap / TileLayer / Tileset
    │
    ├── direct consumer
    │
    └── O_N2::CreateEntitiesForMap()
              │
              ▼
        scene-local ECS registry
```

This separation means gameplay and rendering code can operate on structured map data without needing to parse JSON themselves.

It also leaves room for additional Tiled features to be added later without coupling the rest of the engine directly to Tiled's file formats.

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
└── data/maps/
    └── ...
```

The most important entry points are:

```cpp
map::Tiled::LoadTilemapJSON(...);
map::Tiled::LoadTileLayerJSON(...);
map::Tiled::LoadTilesetJSON(...);
map::O_N2::CreateEntitiesForMap(...);
```
