#pragma once
#include "Scripting/LuaContext.hpp"

namespace ssg::lua::api::std
{
void register_all(LuaContext& context);
}