#include "MapLoader.hpp"

#include <entt/entt.hpp>
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <tinyxml2.h>

#include "Components/CSprite.hpp"
#include "Components/CTexture.hpp"
#include "Components/CTransform.hpp"
#include "EngineContext.hpp"
#include "JsonUtil.hpp"
#include "Logger.hpp"
#include "Systems/AssetManager.hpp"

namespace ssg::map
{

namespace O_N2
{
void CreateEntitiesForMap(entt::registry& registry, Tilemap& tilemap)
{
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

                entt::entity entity = registry.create();

                auto& transform = registry.emplace<CTransform>(entity);
                auto& sprite = registry.emplace<CSprite>(entity);
                auto& texture = registry.emplace<CTexture>(entity);

                transform.position = {static_cast<float>(x * tilemap.getTileWidth()),
                                      static_cast<float>(y * tilemap.getTileHeight())};

                texture.textureID = tileset->getTextureID();
                texture.textureRect = region;
                sprite.size = {static_cast<float>(tilemap.getTileWidth()),
                               static_cast<float>(tilemap.getTileHeight())};
                sprite.zIndex = zIndex;
            }
        }
        zIndex++;
    }
}
} // namespace O_N2

void HandleErrorForJsonProperty(const String& property, const json::json& data,
                                const Filepath& path)
{
    if (!json::Has(data, property))
        throw std::runtime_error(
            std::format("Map from '{}' is missing '{}' property", path.string(), property));
}

MapEntry LookUpMapEntry(const String& field, const Filepath& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file " + filepath.string() + "!");

    json::json data;
    file >> data;

    HandleErrorForJsonProperty(field, data, filepath);
    auto mapField = json::AccessObjectField(data, field);

    HandleErrorForJsonProperty("filepath", mapField, filepath);
    auto configFilepath = json::AttemptAccessField<Filepath>(mapField, "filepath");

    return MapEntry{configFilepath};
}

} // namespace ssg::map

namespace ssg::map::Tiled
{
Tilemap LoadTilemapJSON(EngineContext& context, const Filepath& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file " + path.string() + "!");
    }

    json::json data;
    file >> data;

    HandleErrorForJsonProperty("height", data, path);
    HandleErrorForJsonProperty("width", data, path);
    HandleErrorForJsonProperty("tileheight", data, path);
    HandleErrorForJsonProperty("tilewidth", data, path);
    HandleErrorForJsonProperty("layers", data, path);
    HandleErrorForJsonProperty("tilesets", data, path);

    auto height = json::AttemptAccessField<std::uint32_t>(data, "height");
    auto width = json::AttemptAccessField<std::uint32_t>(data, "width");
    auto tileHeight = json::AttemptAccessField<std::uint32_t>(data, "tileheight");
    auto tileWidth = json::AttemptAccessField<std::uint32_t>(data, "tilewidth");

    Vector<TileLayer> layers;
    Vector<Tileset> sets;
    Vector<ObjectLayer> objectLayers = {}; // TODO: MAKE THIS WORK, UNFINISHED

    for (auto& layerJson : data.at("layers"))
    {
        auto type = json::AttemptAccessField<String>(layerJson, "type");

        if (type == "tilelayer")
        {
            layers.emplace_back(LoadTileLayerJSON(path, layerJson));
        }
        else if (type == "objectgroup")
        {
            // objectLayers.emplace_back(LoadObjectLayer(layerJson, path));
        }
        else
        {
            // image layers, group layers, etc.
        }
    }

    for (auto& tilesetJson : data.at("tilesets"))
    {
        HandleErrorForJsonProperty("firstgid", tilesetJson, path);
        HandleErrorForJsonProperty("source", tilesetJson, path);

        auto firstGid = json::AttemptAccessField<std::uint32_t>(tilesetJson, "firstgid");
        auto source =
            path.parent_path() / json::AttemptAccessField<Filepath>(tilesetJson, "source");

        sets.emplace_back(LoadTilesetJSON(context, source, firstGid));
    }

    return Tilemap{width, height, tileHeight, tileWidth, layers, sets, objectLayers};
}
TileLayer LoadTileLayerJSON(const Filepath& path, const json::json& layerJson)
{
    HandleErrorForJsonProperty("name", layerJson, path);
    HandleErrorForJsonProperty("width", layerJson, path);
    HandleErrorForJsonProperty("height", layerJson, path);
    HandleErrorForJsonProperty("data", layerJson, path);
    HandleErrorForJsonProperty("id", layerJson, path);

    auto layerName = json::AttemptAccessField<String>(layerJson, "name");
    auto layerWidth = json::AttemptAccessField<std::uint32_t>(layerJson, "width");
    auto layerHeight = json::AttemptAccessField<std::uint32_t>(layerJson, "height");
    auto layerData = json::AttemptAccessField<Vector<TileLayer::TileID>>(layerJson, "data");

    return TileLayer{layerName, layerWidth, layerHeight, layerData};
}

Tileset LoadTilesetJSON(EngineContext& context, const Filepath& path, uint32_t firstgid)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file " + path.string() + "!");

    json::json data;
    file >> data;

    HandleErrorForJsonProperty("name", data, path);
    HandleErrorForJsonProperty("tilewidth", data, path);
    HandleErrorForJsonProperty("tileheight", data, path);
    HandleErrorForJsonProperty("tilecount", data, path);
    HandleErrorForJsonProperty("columns", data, path);
    HandleErrorForJsonProperty("type", data, path);
    HandleErrorForJsonProperty("image", data, path);

    // verify its type is a tileset
    if (json::AttemptAccessField<String>(data, "type") != "tileset")
        throw std::runtime_error("Tileset type inconsistent with json data! Filepath: " +
                                 path.string());

    auto name = json::AttemptAccessField<String>(data, "name");
    auto imageSource = json::AttemptAccessField<Filepath>(data, "image");
    auto tileWidth = json::AttemptAccessField<std::uint32_t>(data, "tilewidth");
    auto tileHeight = json::AttemptAccessField<std::uint32_t>(data, "tileheight");
    auto tileCount = json::AttemptAccessField<std::uint32_t>(data, "tilecount");
    auto columns = json::AttemptAccessField<std::uint32_t>(data, "columns");

    TextureHandle textureID = context.assetManager.LoadTexture(imageSource);

    // convert int32_t to uint32_t to prevent implicit conversion
    return Tileset{name, tileWidth, tileHeight, columns, tileCount, firstgid, textureID};
}
} // namespace ssg::map::Tiled