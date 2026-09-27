#pragma once
#include "../LuaContext.hpp"

namespace ssg::lua::api::atlas_loaders
{
void register_texturepacker(LuaContext& lua_context);

void register_all(LuaContext& lua_context);
} // namespace ssg::lua::api::atlas_loaders
