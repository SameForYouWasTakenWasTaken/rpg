#pragma once
#include <sol/state.hpp>

#include "EngineContext.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "Types.hpp"

namespace ssg::lua::api::assets
{
TextureHandle get_texture_handle(EngineContext& context, const Filepath& path);
const sf::Texture& get_texture(EngineContext& context, TextureHandle handle);

void register_all(sol::table& table, EngineContext& context);
} // namespace ssg::lua::api::assets