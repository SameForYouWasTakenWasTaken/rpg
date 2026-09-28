#pragma once
#include "SceneStack.hpp"
#include "Scripting/LuaContext.hpp"

namespace ssg::lua::api::hierarchy
{

void register_attach(LuaContext& context);
void register_all(LuaContext& context, SceneStack& stack, sol::table namespaceTable);
} // namespace ssg::lua::api::hierarchy