#include "MainAPI.hpp"

#include "Components/ComponentView.hpp"
#include "Entity.hpp"
#include "Logger.hpp"
#include "Systems/AssetManager.hpp"
#include "Systems/Input.hpp"
#include "Types/SSGTypes.hpp"

namespace ssg::lua::api
{
sol::table engine_table;

void lua_api_init(sol::state& state, EngineContext& engineContext, ApplicationContext& appContext,
                  SceneStack& stack)
{
    engine_table = state.create_table("engine");
    register_print(engineContext, state);
    register_error(engineContext, state);

    glob_types::register_all(state, engine_table);
    register_component_refs(state, ScriptComponents{});

    assets::register_all(state, engine_table, engineContext);
    input::register_all(engine_table, engineContext);
    entity::register_all(engine_table, stack);

    engine_table.set("Window", std::ref(appContext.MainWindow));
}
void update(sol::state& state, float dt)
{
    const sol::function _update = engine_table["update"];
    _update.call<void>(dt);
}
void register_print(EngineContext& context, sol::state& state)
{
    auto& logger = context.logger;
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
void register_error(EngineContext& context, sol::state& state)
{
    auto& logger = context.logger;
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
