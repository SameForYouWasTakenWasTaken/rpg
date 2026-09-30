#pragma once
#include <sol/state_view.hpp>

#include "Map/Tilemap.hpp"
#include "SceneStack.hpp"
#include "Scripting/LuaContext.hpp"

namespace ssg::lua::api::map_loaders
{
void register_tilelayer(LuaContext& context);
void register_tileset(LuaContext& context);
void register_tilemap(LuaContext& context, SceneStack& stack);
void register_load_tiled(LuaContext& context, sol::table namespaceTable);
void register_all(LuaContext& context, sol::table namespaceTable, SceneStack& stack);
} // namespace ssg::lua::api::map_loaders