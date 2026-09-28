#pragma once
#include <Entity.hpp>
#include <sol/state.hpp>

#include "LuaContext.hpp"
#include "SceneStack.hpp"

namespace ssg::lua::api::entity
{
inline bool valid_current_scene(SceneStack& sceneStack) { return sceneStack.Empty() == false; }
inline entt::registry& current_registry(SceneStack& sceneStack)
{
    return sceneStack.Current()->GetContext().registry;
}
void register_entity(LuaContext& lua_context, SceneStack& sceneStack);
void register_all(LuaContext& lua_context, SceneStack& sceneStack);
} // namespace ssg::lua::api::entity
