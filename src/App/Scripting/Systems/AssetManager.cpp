#pragma once
#include "AssetManager.hpp"

#include "AtlasLoaders.hpp"
#include "Logger.hpp"
#include "Systems/AssetManager.hpp"

namespace ssg::lua::api::assets
{

Texture get_texture(LuaContext& lua_context, const Filepath& path)
{
    auto& context = lua_context.engine_context;
    auto handle = context.assetManager.LoadTexture(path);
    return Texture{handle, context.assetManager.GetTexture(handle)};
}
void register_texture_handle(LuaContext& lua_context)
{
    auto table = lua_context.namespace_table["assets"].get<sol::table>();
    table.new_usertype<TextureHandle>(
        "TextureHandle", sol::no_constructor,

        "id", sol::readonly(&TextureHandle::id),

        "valid", &TextureHandle::IsValid,

        "get_invalid_value", &TextureHandle::GetInvalidValue,

        sol::meta_function::to_string,
        [](const TextureHandle& handle) { return std::to_string(handle.id); },

        sol::meta_function::equal_to,
        [](const TextureHandle& a, const TextureHandle& b) { return a == b; }

    );
}
void register_texture(LuaContext& lua_context)
{
    auto table = lua_context.namespace_table["assets"].get<sol::table>();
    table.new_usertype<Texture>(
        "Texture", sol::no_constructor,

        "width", sol::readonly_property([](const Texture& t) { return t.texture.getSize().x; }),

        "height", sol::readonly_property([](const Texture& t) { return t.texture.getSize().y; }),

        "smooth", sol::readonly_property([](const Texture& t) { return t.texture.isSmooth(); }),

        "repeated", sol::readonly_property([](const Texture& t) { return t.texture.isRepeated(); }),

        "srgb", sol::readonly_property([](const Texture& t) { return t.texture.isSrgb(); }),

        "native_handle",
        sol::readonly_property([](const Texture& t) { return t.texture.getNativeHandle(); }),

        "asset_handle", sol::readonly_property([](const Texture& t) { return t.handle; }),

        "size",
        [](const Texture& t)
        {
            const auto size = t.texture.getSize();
            return Vec2(size.x, size.y);
        },

        sol::meta_function::equal_to, [](const Texture& a, const Texture& b) { return a == b; }

    );
}
void register_atlas(LuaContext& lua_context)
{
    auto& state = lua_context.state;
    auto table = lua_context.namespace_table["assets"].get<sol::table>();
    // Regions are exclusive to atlasses, but they function as sf::FloatRect's, so no construction
    // needed
    state.new_usertype<Region>("Region", sol::no_constructor,

                               "position", &Region::position,

                               "size", &Region::size,

                               "get_dimensions",
                               [](const Region& region)
                               {
                                   return sf::FloatRect({region.position.x, region.position.y},
                                                        {region.size.x, region.size.y});
                               });

    table.new_usertype<Atlas>(
        "Atlas", sol::no_constructor,

        "id", sol::readonly_property([](const Atlas& atlas) { return atlas.GetID(); }),

        "asset_handle",
        sol::readonly_property([](const Atlas& atlas) { return atlas.GetTextureID(); }),

        "get_region",
        [](const Atlas& atlas, const String& name) -> sol::optional<Region>
        {
            auto region = atlas.GetRegion(name);
            if (region)
                return region.value();
            return sol::nullopt;
        },

        "get_regions",
        [&state](const Atlas& atlas)
        {
            sol::table regionsTable = state.create_table();

            for (const auto& [key, value] : atlas.GetAllRegions())
            {
                regionsTable[key] = value;
            }
        });

    // load the atlas loaders
    atlas_loaders::register_all(lua_context);
}
void register_all(LuaContext& lua_context)
{
    auto& table = lua_context.namespace_table;
    auto assets_table = table.create_named("assets");

    register_texture_handle(lua_context);
    register_texture(lua_context);
    register_atlas(lua_context);
    assets_table.set_function("get_texture", [&lua_context](const std::string& path)
                              { return get_texture(lua_context, Filepath(path)); });
}
} // namespace ssg::lua::api::assets
