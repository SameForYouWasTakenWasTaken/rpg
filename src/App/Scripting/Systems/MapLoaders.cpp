#include "MapLoaders.hpp"

#include "Factories/Gameplay/Map/MapLoader.hpp"
namespace ssg::lua::api::map_loaders
{
void register_tilelayer(LuaContext& context)
{
    context.state.new_usertype<map::TileLayer>(
        "Tilelayer", sol::no_constructor,

        "name", sol::readonly_property(&map::TileLayer::getName),

        "width", sol::readonly_property(&map::TileLayer::getWidth),

        "height", sol::readonly_property(&map::TileLayer::getHeight),

        "tiles", sol::readonly_property(&map::TileLayer::getTiles), "at",
        [](const map::TileLayer& layer, std::uint32_t x, std::uint32_t y) -> map::TileLayer::TileID
        { return layer.at(x, y); }

    );
}
void register_tileset(LuaContext& context)
{
    context.state.new_usertype<map::Tileset>(
        "Tileset", sol::no_constructor,

        "columns", sol::readonly_property(&map::Tileset::getColumns),

        "name", sol::readonly_property(&map::Tileset::getName),

        "width", sol::readonly_property(&map::Tileset::getWidth),

        "height", sol::readonly_property(&map::Tileset::getHeight),

        "tile_count", sol::readonly_property(&map::Tileset::getTileCount),

        "first_gid", sol::readonly_property(&map::Tileset::getFirstGid),

        "asset_handle", sol::readonly_property(&map::Tileset::getTextureID),

        "get_region", [](const map::Tileset& tileset, map::TileLayer::TileID localId) -> Region
        { return tileset.getRegion(localId); });
}
void register_tilemap(LuaContext& context, SceneStack& stack)
{
    context.state.new_usertype<map::Tilemap>(
        "Tilemap", sol::no_constructor, "width", sol::readonly_property(&map::Tilemap::getWidth),
        "height", sol::readonly_property(&map::Tilemap::getHeight),

        "tile_width", sol::readonly_property(&map::Tilemap::getTileWidth),

        "tile_height", sol::readonly_property(&map::Tilemap::getTileHeight),

        "tile_layers", sol::readonly_property(&map::Tilemap::getTileLayers),

        "tilesets", sol::readonly_property(&map::Tilemap::getTilesets), "get_tileset_for_gid",
        [](const map::Tilemap& map,
           map::TileLayer::TileID gid) -> sol::optional<const map::Tileset&>
        {
            const auto* set = map.getTilesetForGid(gid);
            if (!set)
                return sol::nullopt;
            return *set;
        },

        "create",
        [&stack](map::Tilemap& map)
        {
            auto* currentScene = stack.Current();
            if (!currentScene)
                return;

            auto& registry = currentScene->GetContext().registry;

            map::O_N2::CreateEntitiesForMap(registry, map);
        }

    );
}

void register_load_tiled(LuaContext& context, sol::table namespaceTable)
{
    namespaceTable.set_function("load_tiled",
                                [&context](std::string_view filepath)
                                {
                                    map::Tilemap tilemap = map::Tiled::LoadTilemapJSON(
                                        context.engine_context, Filepath{filepath});
                                    return tilemap;
                                });
}
void register_all(LuaContext& context, sol::table namespaceTable, SceneStack& stack)
{
    sol::table mapTable = namespaceTable.create_named("map");

    register_tilelayer(context);
    register_tileset(context);
    register_tilemap(context, stack);

    register_load_tiled(context, mapTable);
}
} // namespace ssg::lua::api::map_loaders