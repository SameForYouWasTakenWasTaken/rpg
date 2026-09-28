#pragma once
#include <Entity.hpp>
#include <sol/state.hpp>

#include "LuaContext.hpp"
#include "SceneStack.hpp"

namespace ssg::lua::api::entity
{

void register_entity(LuaContext& lua_context, SceneStack& sceneStack);
void register_all(LuaContext& lua_context, SceneStack& sceneStack);
} // namespace ssg::lua::api::entity
