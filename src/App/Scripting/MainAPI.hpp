#pragma once
#include <sol/sol.hpp>

#include "Engine.hpp"
#include "LuaContext.hpp"
#include "SceneStack.hpp"

namespace ssg::lua::api
{
struct ScriptLocation
{
    std::string file;
    int line = -1;
};

inline ScriptLocation current_script_location(lua_State* L)
{
    lua_Debug ar;
    for (int level = 1; lua_getstack(L, level, &ar); ++level)
    {
        lua_getinfo(L, "Sl", &ar);
        if (ar.currentline > 0) // skip C frames (e.g. print called through pcall)
        {
            std::string file = ar.source[0] == '@' ? ar.source + 1 : ar.short_src;
            return {std::move(file), ar.currentline};
        }
    }
    return {};
}

inline LuaContext create_lua_context(EngineContext& engine_context,
                                     ApplicationContext& application_context)
{
    auto& lua_state = engine_context.engine.GetScriptEngine().getState();
    auto engine_table = lua_state.create_table("engine");
    return LuaContext{lua_state, engine_table, engine_context, application_context};
}

void lua_api_init(LuaContext& lua_context, SceneStack& stack);
void update(LuaContext& lua_context, float dt);
void register_print(LuaContext& lua_context);
void register_error(LuaContext& lua_context);
} // namespace ssg::lua::api
