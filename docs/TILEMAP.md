# 🗺️ Tilemap System

This document describes the tilemap implementation used by the RPG project: how maps are authored in Tiled, how external
tilesets are loaded, how tile IDs are resolved, and how the resulting map data can be consumed by the ECS renderer.

The implementation is intentionally focused on the data-loading foundation. Advanced Tiled features are not part of the
current runtime contract.

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
       Tileset (.tsx) + image
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
          ECS tile data
                │
                ▼
             Renderer
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

The engine currently consumes Tiled's JSON map format (`.tmj`). The supported map configuration is a finite, orthogonal
map containing normal tile layers and references to external tilesets.

Tiled supports many additional features that are not currently consumed by the loader, including object layers, image
layers, group layers, infinite maps, custom properties, tile animations, and tile transformations.

### TexturePacker

[TexturePacker](https://www.codeandweb.com/texturepacker) is used to create texture atlases.

Official documentation:

- [TexturePacker Documentation](https://www.codeandweb.com/texturepacker/documentation)
- [TexturePacker Support](https://www.codeandweb.com/texturepacker/support)

The tilemap loader does not parse TexturePacker's JSON metadata. It loads the image referenced by the tileset and
calculates tile regions from the tileset's grid metadata.

---

## 📁 Asset Layout

External tilesets should be kept alongside the map that references them. This is especially useful when maps and their
assets are moved as a package.

For example:

```text
assets/
└── data/
    └── maps/
        └── forest/
            ├── forest.tmj
            └── forest.tsx
```

The map references the tileset through a `source` entry:

```json
{
    "firstgid": 1,
    "source": "forest.tsx"
}
```

The external tileset contains its metadata and image reference:

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

### Path handling

The `.tsx` path is resolved relative to the directory containing the `.tmj` file.

The image `source` from the `.tsx` is currently passed directly to `AssetManager::LoadTexture()`. Therefore, the image
path must match the project's asset-loading path expectations.

Keeping the map and external tileset together avoids unnecessary complexity when importing a complete map package. The
`source` field is the part that must remain correct when the directory is moved.

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

A GID of `0` represents an empty tile and does not produce a rendered tile.

### `Tileset`

A `Tileset` contains the information required to resolve a global Tiled tile ID into a texture and texture region.

It stores:

- tileset name
- tile width
- tile height
- number of columns
- tile count
- `firstgid`
- the engine's `TextureID`

The current public API includes:

```cpp
std::uint32_t getColumns() const;
String getName() const;
std::uint32_t getWidth() const;
std::uint32_t getHeight() const;
std::uint32_t getTileCount() const;
std::uint32_t getFirstGid() const;
TextureID getTextureID() const;

Region getRegion(std::uint32_t localId) const;
```

---

## 🔢 Global Tile IDs and `firstgid`

Tiled stores tile IDs in map layers as **global IDs (GIDs)**. A GID is not directly an index into one particular
tileset.

Each tileset has a `firstgid` assigned by the map.

For example:

```text
Tileset A: firstgid = 1
Tileset B: firstgid = 1981
Tileset C: firstgid = 3961
```

Given a GID, the loader finds the tileset whose `firstgid` is the greatest value that is still less than or equal to the
GID.

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
columns   = 44
tileWidth = 32
tileHeight = 32
localId   = 45
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

This calculation assumes the image is laid out as a regular grid matching the tileset's `tilewidth`, `tileheight`, and
`columns` values.

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

The `source` is combined with the map's parent directory and passed to the XML tileset loader.

### 5. Parse the `.tsx`

The external tileset is parsed with `tinyxml2`.

The loader reads these attributes from the `<tileset>` element:

- `name`
- `tilewidth`
- `tileheight`
- `tilecount`
- `columns`

It then reads the `<image source="...">` element and loads the referenced texture through the engine's `AssetManager`.

The resulting `Tileset` stores the parsed metadata, the map's `firstgid`, and the resulting `TextureID`.

### 6. Return the runtime representation

The loader returns one `Tilemap` containing all successfully loaded tile layers and tilesets.

The object-layer collection is currently empty because object-group parsing has not been implemented.

---

## 🖼️ Consuming Tilemap Data

The tilemap classes intentionally do not depend on ECS entity creation. A consumer can iterate through the layers and
resolve each non-empty GID independently.

A simplified example is:

```cpp
map::Tilemap tilemap =
    map::Tiled::LoadTilemapJSON(
        m_EngineContext,
        "data/maps/forest/forest.tmj"
    );

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

The current loader consumes the map dimensions, tile layers, and external tileset references.

### `.tsx`

The XML external tileset format produced by Tiled.

The current loader consumes the tileset's basic grid metadata and its image source.

### Atlas image

The image referenced by the external tileset is loaded as the texture used for the tiles.

The current loader does not consume TexturePacker JSON metadata; the tileset's grid information is sufficient for region
calculation.

---

## ⚠️ Current Limitations

Currently supported:

- finite orthogonal maps
- external `.tsx` tilesets
- multiple tile layers
- multiple tilesets
- global GID / `firstgid` resolution
- regular grid-based texture-region calculation
- loading tileset textures through `AssetManager`

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

Maps should therefore stay within the supported subset of Tiled until the corresponding loader functionality is
implemented.

---

## 🧭 Design Direction

The tilemap implementation keeps file-format parsing separate from the runtime map representation:

```text
Tiled files
    │
    ▼
MapLoader
    │
    ▼
Tilemap / TileLayer / Tileset
    │
    ▼
Consumer / ECS renderer
```

This separation means gameplay and rendering code can operate on structured map data without needing to parse JSON or
XML themselves.

It also leaves room for additional Tiled features to be added later without coupling the rest of the engine directly to
Tiled's file formats.

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
map::Tiled::LoadTilesetXML(...);
```
