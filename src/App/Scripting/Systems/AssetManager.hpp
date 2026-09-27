#pragma once
#include "../LuaContext.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SceneStack.hpp"
#include "Scripting/Types/Texture.hpp"
#include "Types.hpp"

namespace ssg::lua::api::assets
{

TextureHandle get_texture_handle(LuaContext& lua_context, const Filepath& path);
Texture get_texture(LuaContext& lua_context, const Filepath& path);

void register_texture_handle(LuaContext& lua_context);
void register_texture(LuaContext& lua_context);
void register_atlas(LuaContext& lua_context);
void register_all(LuaContext& lua_context, SceneStack& stack);
} // namespace ssg::lua::api::assets
