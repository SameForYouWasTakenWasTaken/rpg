#pragma once
#include <sol/state.hpp>

namespace ssg::lua::api::glob_types
{
void register_vec2(sol::state& state);
void register_camera(sol::state& state);
void register_sf_float_rect(sol::state& state);
void register_window(sol::state& state);
void register_all(sol::state& state, sol::table table);
} // namespace ssg::lua::api::glob_types