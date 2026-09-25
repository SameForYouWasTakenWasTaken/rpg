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

void register_all(sol::table& table, EngineContext& context);
} // namespace ssg::lua::api::assets