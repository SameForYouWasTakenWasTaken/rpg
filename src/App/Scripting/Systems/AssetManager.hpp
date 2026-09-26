#pragma once
#include <sol/state.hpp>

#include "EngineContext.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "Scripting/Types/Texture.hpp"
#include "Types.hpp"

namespace ssg::lua::api::assets
{

TextureHandle get_texture_handle(EngineContext& context, const Filepath& path);
Texture get_texture(EngineContext& context, const Filepath& path);

void register_texture_handle(sol::table& table, EngineContext& context);
void register_texture(sol::table& table, EngineContext& context);
void register_atlas(sol::state& state, sol::table& table, EngineContext& context);
void register_all(sol::state& state, sol::table& table, EngineContext& context);
} // namespace ssg::lua::api::assets