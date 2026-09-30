#pragma once
#include "Scripting/LuaContext.hpp"

namespace ssg::lua::api::lib
{
void register_all(LuaContext& context);
} // namespace ssg::lua::api::lib