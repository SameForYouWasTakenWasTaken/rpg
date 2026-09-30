#pragma once
#include <sol/state_view.hpp>
#include <utility>

#include "ApplicationContext.hpp"
#include "EngineContext.hpp"

namespace ssg::lua
{
struct LuaContext
{
    sol::state_view state;
    sol::table namespace_table;
    EngineContext& engine_context;
    ApplicationContext& application_context;

    explicit LuaContext(sol::state_view state, sol::table engine_table, EngineContext& context,
                        ApplicationContext& application_context)
        : state(std::move(state)), namespace_table(std::move(engine_table)),
          engine_context(context), application_context(application_context)
    {
    }
};
} // namespace ssg::lua