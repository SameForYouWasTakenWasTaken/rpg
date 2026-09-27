#pragma once
#include <sol/state.hpp>

#include "../../../Core/Scripting/Signals/Signal.hpp"
#include "../LuaContext.hpp"
#include "Application.hpp"

namespace ssg::lua::api::glob_types
{
void register_vec2(sol::state_view state);
void register_camera(sol::state_view state);
void register_sf_float_rect(sol::state_view state);
void register_window(sol::state_view state);
void register_all(LuaContext& context);

namespace events::window
{
void on_resize(LuaContext& context, const WindowResizeEvent& e);
void on_close(LuaContext& context, const WindowCloseEvent& e);
} // namespace events::window
} // namespace ssg::lua::api::glob_types