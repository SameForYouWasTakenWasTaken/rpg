#include "MainAPI.hpp"

#include "Components/ComponentView.hpp"
#include "Entity.hpp"
#include "Logger.hpp"
#include "Scripting/Signals/Signal.hpp"
#include "Systems/AssetManager.hpp"
#include "Systems/Hierarchy.hpp"
#include "Systems/Input.hpp"
#include "Types/SSGTypes.hpp"

namespace ssg::lua::api
{
void lua_api_init(LuaContext& lua_context, SceneStack& stack)
{
    register_print(lua_context);
    register_error(lua_context);

    signals::register_all(lua_context.state);
    glob_types::register_all(lua_context);

    register_component_refs(lua_context, ScriptComponents{});

    assets::register_all(lua_context, stack);
    input::register_all(lua_context);
    entity::register_all(lua_context, stack);
    hierarchy::register_all(lua_context, stack);
}
void update(LuaContext& lua_context, float dt)
{
    auto& scriptEngine = lua_context.engine_context.engine.GetScriptEngine();
    auto& scripts = scriptEngine.getScripts();
    for (auto& script : scripts)
    {
        sol::optional<sol::function> update = script.environment["update"];

        if (update)
        {
            auto result = (*update)(dt);

            if (!result.valid())
            {
                sol::error err = result;
                std::cerr << "[lua] update error: " << err.what() << '\n';
            }
        }
    }
}
void register_print(LuaContext& lua_context)
{
    auto& logger = lua_context.engine_context.logger;
    auto& state = lua_context.state;
    state.set_function("print",
                       [&logger](sol::this_state ts, sol::variadic_args args)
                       {
                           lua_State* L = ts;
                           std::string line;
                           bool first = true;

                           for (auto arg : args)
                           {
                               if (!first)
                                   line += '\t';
                               first = false;

                               size_t len = 0;
                               const char* s = luaL_tolstring(L, arg.stack_index(), &len);
                               line.append(s, len);
                               lua_pop(L, 1); // luaL_tolstring pushes its result
                           }

                           ScriptLocation loc = current_script_location(L);
                           logger.Info(log::LogLocation(loc.file, static_cast<uint32_t>(loc.line)),
                                       "lua", "{}", line);
                       });
}
void register_error(LuaContext& lua_context)
{
    auto& logger = lua_context.engine_context.logger;
    auto& state = lua_context.state;
    state.set_function("error",
                       [&logger](sol::this_state ts, sol::variadic_args args)
                       {
                           lua_State* L = ts;
                           std::string line;
                           bool first = true;

                           for (auto arg : args)
                           {
                               if (!first)
                                   line += '\t';
                               first = false;

                               size_t len = 0;
                               const char* s = luaL_tolstring(L, arg.stack_index(), &len);
                               line.append(s, len);
                               lua_pop(L, 1); // luaL_tolstring pushes its result
                           }

                           ScriptLocation loc = current_script_location(L);
                           logger.Error(log::LogLocation(loc.file, static_cast<uint32_t>(loc.line)),
                                        "lua", "{}", line);
                       });
}
} // namespace ssg::lua::api
