#pragma once
#include <sol/state.hpp>
#include <sol/table.hpp>

#include "EngineContext.hpp"

namespace ssg::lua::api::atlas_loaders
{
void register_texturepacker(sol::state& state, sol::table table, EngineContext& context);

void register_all(sol::state& state, sol::table table, EngineContext& context);
} // namespace ssg::lua::api::atlas_loaders