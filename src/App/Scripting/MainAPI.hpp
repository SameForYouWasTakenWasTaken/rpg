#pragma once
#include <sol/sol.hpp>

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

void lua_api_init(sol::state& state, EngineContext& context, SceneStack& stack);
void update(sol::state& state, float dt);
void register_print(EngineContext& context, sol::state& state);
} // namespace ssg::lua::api