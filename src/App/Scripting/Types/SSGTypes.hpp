#pragma once
#include <sol/state.hpp>

namespace ssg::lua::api::glob_types
{
void register_vec2(sol::state& state);
void register_all(sol::state& state);
} // namespace ssg::lua::api::glob_types