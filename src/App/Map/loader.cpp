#pragma once

#include "loader.hpp"

#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <tinyxml2.h>

#include "EngineContext.hpp"
#include "JsonUtil.hpp"
#include "Logger.hpp"
#include "Systems/AssetManager.hpp"

namespace ssg::map
{

void HandleErrorForJsonProperty(const String& property, const json::json& data,
                                const Filepath& path)
{
    if (!json::Has(data, property))
        throw std::runtime_error(
            std::format("Map from '{}' is missing '{}' property", path.string(), property));
}

Tilemap Tiled::LoadTilemap(EngineContext& context, const Filepath& path)
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
            layers.emplace_back(LoadTileLayer(path, layerJson));
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

        sets.emplace_back(LoadTileset(context, source, firstGid));
    }

    return Tilemap{width, height, tileHeight, tileWidth, layers, sets, objectLayers};
}
TileLayer Tiled::LoadTileLayer(const Filepath& path, const json::json& layerJson)
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

Tileset Tiled::LoadTileset(EngineContext& context, const Filepath& path, uint32_t firstgid)
{
    using namespace tinyxml2;

    XMLDocument doc;
    XMLError result = doc.LoadFile(path.string().c_str());

    if (result != XML_SUCCESS)
        throw std::runtime_error("Could not load tileset from " + path.string());

    XMLElement* tileset = doc.FirstChildElement("tileset");

    if (!tileset)
        throw std::runtime_error("Missing <tileset> element in " + path.string());

    const char* name;
    std::int32_t tileWidth;
    std::int32_t tileHeight;
    std::int32_t tileCount;
    std::int32_t columns;

    if (tileset->QueryStringAttribute("name", &name) != XML_SUCCESS)
        throw std::runtime_error("Missing or invalid 'name' attribute");

    if (tileset->QueryIntAttribute("tilewidth", &tileWidth) != XML_SUCCESS)
        throw std::runtime_error("Missing or invalid 'tilewidth' tileset attribute");

    if (tileset->QueryIntAttribute("tileheight", &tileHeight) != XML_SUCCESS)
        throw std::runtime_error("Missing or invalid 'tileheight' tileset attribute");

    if (tileset->QueryIntAttribute("tilecount", &tileCount) != XML_SUCCESS)
        throw std::runtime_error("Missing or invalid 'tilecount' tileset attribute");

    if (tileset->QueryIntAttribute("columns", &columns) != XML_SUCCESS)
        throw std::runtime_error("Missing or invalid 'columns' tileset attribute");

    // convert int32_t to uint32_t to prevent implicit conversion
    return Tileset{std::string(name),
                   static_cast<std::uint32_t>(tileWidth),
                   static_cast<std::uint32_t>(tileHeight),
                   static_cast<std::uint32_t>(columns),
                   static_cast<std::uint32_t>(tileCount),
                   firstgid,
                   static_cast<TextureID>(-1)};
}
} // namespace ssg::map