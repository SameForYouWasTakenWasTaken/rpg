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

        sets.emplace_back(LoadTilesetXML(context, source, firstGid));
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

Tileset LoadTilesetXML(EngineContext& context, const Filepath& path, uint32_t firstgid)
{
    using namespace tinyxml2;

    XMLDocument doc;
    XMLError result = doc.LoadFile(path.string().c_str());

    if (result != XML_SUCCESS)
        throw std::runtime_error("Could not load tileset from " + path.string());

    XMLElement* tilesetElement = doc.FirstChildElement("tileset");

    if (!tilesetElement)
        throw std::runtime_error("Missing <tileset> element in " + path.string());

    const char* name;
    std::int32_t tileWidth;
    std::int32_t tileHeight;
    std::int32_t tileCount;
    std::int32_t columns;

    if (tilesetElement->QueryStringAttribute("name", &name) != XML_SUCCESS)
        throw std::runtime_error("Missing or invalid 'name' attribute");

    if (tilesetElement->QueryIntAttribute("tilewidth", &tileWidth) != XML_SUCCESS)
        throw std::runtime_error("Missing or invalid 'tilewidth' tileset attribute");

    if (tilesetElement->QueryIntAttribute("tileheight", &tileHeight) != XML_SUCCESS)
        throw std::runtime_error("Missing or invalid 'tileheight' tileset attribute");

    if (tilesetElement->QueryIntAttribute("tilecount", &tileCount) != XML_SUCCESS)
        throw std::runtime_error("Missing or invalid 'tilecount' tileset attribute");

    if (tilesetElement->QueryIntAttribute("columns", &columns) != XML_SUCCESS)
        throw std::runtime_error("Missing or invalid 'columns' tileset attribute");

    // parse image source
    const char* imageSource;
    XMLElement* imageElement = tilesetElement->FirstChildElement("image");

    if (imageElement->QueryStringAttribute("source", &imageSource) != XML_SUCCESS)
        throw std::runtime_error("Missing or invalid 'source' attribute");

    auto textureID = context.assetManager.LoadTexture(Filepath(imageSource));

    // convert int32_t to uint32_t to prevent implicit conversion
    return Tileset{std::string(name),
                   static_cast<std::uint32_t>(tileWidth),
                   static_cast<std::uint32_t>(tileHeight),
                   static_cast<std::uint32_t>(columns),
                   static_cast<std::uint32_t>(tileCount),
                   firstgid,
                   textureID};
}
} // namespace ssg::map::Tiled