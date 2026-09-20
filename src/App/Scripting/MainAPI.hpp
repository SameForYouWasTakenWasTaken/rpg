#pragma once
#include <sol/sol.hpp>

#include "SceneStack.hpp"

namespace ssg::lua::api
{
void lua_api_init(sol::state& state, EngineContext& context, SceneStack& stack);
void update(sol::state& state, float dt);
} // namespace ssg::lua::api