#pragma once
#include "SceneStack.hpp"
#include "Scripting/LuaContext.hpp"

namespace ssg::lua::api::hierarchy
{

void helper_attach(LuaContext& context, entt::registry& r, Entity parent, Entity child);
void helper_detach(LuaContext& context, entt::registry& r, Entity child);
void register_all(LuaContext& context, SceneStack& stack);
} // namespace ssg::lua::api::hierarchy