#pragma once
#include "AssetManager.hpp"

#include "Systems/AssetManager.hpp"

namespace ssg::lua::api::assets
{

TextureHandle get_texture_handle(EngineContext& context, const Filepath& path)
{
    return context.assetManager.LoadTexture(path);
}
const sf::Texture& get_texture(EngineContext& context, TextureHandle handle)
{
    return context.assetManager.GetTexture(handle);
}
void register_all(sol::table& table, EngineContext& context)
{
    auto assets_table = table.create_named("assets");
    assets_table.new_usertype<TextureHandle>("TextureHandle", sol::no_constructor,

                                             "id", sol::readonly(&TextureHandle::id),

                                             "valid", &TextureHandle::IsValid,

                                             "get_invalid_value", &TextureHandle::GetInvalidValue);
    assets_table.new_usertype<sf::Texture>(
        "Texture", sol::no_constructor,
        "width", sol::readonly_property([](const sf::Texture& t) { return t.getSize().x; }),
        "height", sol::readonly_property([](const sf::Texture& t) { return t.getSize().y; }),

        "smooth", sol::readonly_property(&sf::Texture::isSmooth), "repeated",
        sol::readonly_property(&sf::Texture::isRepeated), "srgb",
        sol::readonly_property(&sf::Texture::isSrgb),

        "native_handle", sol::readonly_property(&sf::Texture::getNativeHandle),

        "size",
        [](const sf::Texture& t)
        {
            auto s = t.getSize();
            return Vec2(s.x, s.y); // two return values in Lua
        });

    assets_table.set_function("get_texture_handle", [&context](const std::string& path)
                              { return get_texture_handle(context, path); });
    assets_table.set_function("get_texture", [&context](TextureHandle handle)
                              { return get_texture(context, handle); });
}
} // namespace ssg::lua::api::assets