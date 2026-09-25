#pragma once
#include "AssetManager.hpp"

#include "Systems/AssetManager.hpp"

namespace ssg::lua::api::assets
{

Texture get_texture(EngineContext& context, const Filepath& path)
{
    auto handle = context.assetManager.LoadTexture(path);
    return Texture{handle, context.assetManager.GetTexture(handle)};
}
void register_all(sol::table& table, EngineContext& context)
{
    auto assets_table = table.create_named("assets");
    assets_table.new_usertype<TextureHandle>(
        "TextureHandle", sol::no_constructor,

        "id", sol::readonly(&TextureHandle::id),

        "valid", &TextureHandle::IsValid,

        "get_invalid_value", &TextureHandle::GetInvalidValue,

        sol::meta_function::to_string,
        [](const TextureHandle& handle) { return std::to_string(handle.id); },

        sol::meta_function::equal_to,
        [](const TextureHandle& a, const TextureHandle& b) { return a == b; }

    );
    assets_table.new_usertype<Texture>(
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

    assets_table.set_function("get_texture", [&context](const std::string& path)
                              { return get_texture(context, Filepath(path)); });
}
} // namespace ssg::lua::api::assets